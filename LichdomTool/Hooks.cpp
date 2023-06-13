#include <Windows.h>

#include "CryLich.h"
#include "Util.h"
#include "Main.h"
#include "MinHook.h"
#include <WinUser.h>
#include <d3d11.h>
#include <unordered_map>
#include <DirectXMath.h>


#pragma comment(lib, "libMinHook.x64.lib")

typedef DWORD(WINAPI* tIDXGISwapChain_Present)(IDXGISwapChain*, UINT, UINT);
typedef BOOL(WINAPI* tSetCursorPos)(int x, int y);

typedef bool(__fastcall* tDevMode)(CryLich::ISystem* system);
typedef void(__fastcall* tUpdateView) (CryLich::CView* pThis, float frameTime, bool isActive);
typedef void(__fastcall* tUpdateKeyboard) (CryLich::CKeyboard* pThis, bool bFocus);
typedef void(__fastcall* tUpdateMouse) (CryLich::CMouse* pThis, bool bFocus, float a3);
typedef void(__fastcall* tUpdateGamepad) (CryLich::CXInputDevice* pThis, bool bFocus);
typedef __int64(__fastcall* tGameUpdate)(CryLich::CGame* pThis,__int64 haveFocus, unsigned int updateFlags);
typedef void(__fastcall* tUpdateEngine)(CryLich::C3DEngine* pThis);
typedef void(__fastcall* tUpdateUI)(CryLich::CFlashUI* pThis);
typedef bool(__fastcall* tUpdateSystem)(CryLich::CSystem* pThis, char updateFlags, int nPauseMode);
typedef void(__fastcall* tSetViewport)(CryLich::CD3D9Renderer* pThis, int x, int y, int width, int height);
typedef bool(__fastcall* tPreUpdate)(CryLich::CCryAction* pThis, bool haveFocus, int updateFlags);
typedef char (__fastcall* tChangeResolution)(CryLich::CD3D9Renderer* pThis, int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen, bool bForceReset);
typedef bool(__fastcall* tOnInputEvent)(CryLich::CHardwareMouse* pthis, const CryLich::SInputEvent* rInputEvent);

tUpdateView oUpdateView = nullptr;
tGameUpdate oGameUpdate = nullptr;
tUpdateEngine oUpdateEngine = nullptr;
tUpdateSystem oUpdateSystem = nullptr;
tSetViewport oSetViewport = nullptr;
tPreUpdate oPreUpdate = nullptr;
tChangeResolution oChangeResolution = nullptr;


tUpdateUI oUpdateUI = nullptr;

tUpdateKeyboard oUpdateKeyboard = nullptr;
tUpdateMouse oUpdateMouse = nullptr;
tUpdateGamepad oUpdateGamepad = nullptr;
tIDXGISwapChain_Present oIDXGISwapChain_Present = nullptr;
tSetCursorPos oSetCursorPos = nullptr;
tOnInputEvent oOnInputEvent = nullptr;

extern CryLich::SCVars* g_pGameCVars;



DWORD WINAPI hIDXGISwapChain_Present(IDXGISwapChain* pSwapchain, UINT SyncInterval, UINT Flags)
{
    if (!g_shutdown)
        g_mainHandle->GetUI()->Draw();

    return oIDXGISwapChain_Present(pSwapchain, SyncInterval, Flags);
}


bool __fastcall hookDevMode(CryLich::ISystem* system) {
    return true;
}

bool __fastcall hookSystemUpdate(CryLich::CSystem* pThis, char updateFlags, int nPauseMode) {

    g_mainHandle->GetCameraManager()->crySystem = pThis;

    return oUpdateSystem(pThis, updateFlags, nPauseMode);

}

__int64 __fastcall hookGameUpdate(CryLich::CGame* pThis, __int64 haveFocus, unsigned int updateFlags) {

    //In order to grab the CVars by the familiar strings we know and love, you have to hook the IConsole object, 
    //because this function is in its vtable. Luckily this is one of the pointers in the CGame object.

    int enabled = 1;
    int disabled = 0;
    int big = 99999;

    g_pGameCVars = pThis->m_pCVars;
    g_mainHandle->GetCameraManager()->cryConsole = pThis->m_pConsole;

    CryLich::ICVar* customResMaxSize = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("r_CustomResMaxSize");
    customResMaxSize->Set(big);

    CryLich::ICVar* con_restricted = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("con_restricted");
    con_restricted->Set(disabled);

    CryLich::ICVar* showDebug = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("xav_showDebugOptions");
    showDebug->Set(enabled);

    CryLich::ICVar* enableCheats = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("xav_activateCheats");
    enableCheats->Set(enabled);

    CryLich::ICVar* cursorFreedom = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("xav_freedomcursor");
    cursorFreedom->Set(enabled); //This doesn't seem to help the cursor issue either

    
    return oGameUpdate(pThis, haveFocus, updateFlags);
}

void __fastcall hookUpdateEngine(CryLich::C3DEngine* pThis) {

    g_mainHandle->GetCameraManager()->cry3DEngine = pThis;

    return oUpdateEngine(pThis);
}

bool __fastcall hookPreUpdate(CryLich::CCryAction* pThis, bool haveFocus, int updateFlags) {

    g_mainHandle->GetCameraManager()->cryAction = pThis;

    return oPreUpdate(pThis, haveFocus, updateFlags);
}

void __fastcall hookSetViewport(CryLich::CD3D9Renderer* pThis, int x, int y, int width, int height) {

    g_mainHandle->GetCameraManager()->cryD3D9Renderer = pThis;

    //The following variable will allow you to set the internal res to anything you want
    g_mainHandle->GetCameraManager()->cryD3D9Renderer->m_prefMonHeight = 99999;
    g_mainHandle->GetCameraManager()->cryD3D9Renderer->m_prefMonWidth = 99999;

    return oSetViewport(pThis, x, y, width, height);
}

void __fastcall hookUpdateUI(CryLich::CFlashUI* pThis) {

    g_mainHandle->GetCameraManager()->cryUI = pThis;

    return oUpdateUI(pThis);
}

void __fastcall hookUpdateView(CryLich::CView* pThis, float frameTime, bool isActive) {

    //CView::Update is the camera function in this game. Not sure about other CryEngine games yet.
    //Just worth noting you aren't looking for anything with "camera" in it, possibly.

    DirectX::XMFLOAT3X4 gameCamera = pThis->m_camera.m_Matrix;
    DirectX::XMFLOAT3X4& gameCameraAdd = pThis->m_camera.m_Matrix;

    g_mainHandle->GetCameraManager()->m_gameMatrixLocation = &gameCameraAdd;
    g_mainHandle->GetCameraManager()->cryCamera = &pThis->m_camera;
    g_mainHandle->GetCameraManager()->cryView = pThis;

    DirectX::XMMATRIX gameMatrix = DirectX::XMLoadFloat3x4(&gameCamera);

    //This game uses a 3x4 column major matrix, where the last column is the position,
    //and the first three are a 3x3 look matrix
    
    if (g_mainHandle->GetCameraManager()->IsCameraEnabled()) {
 
        
        gameMatrix = g_mainHandle->GetCameraManager()->GetMatrix();
        DirectX::XMFLOAT3X4 gameTransform;
        DirectX::XMStoreFloat3x4(&gameTransform, gameMatrix);
        gameTransform = g_mainHandle->GetCameraManager()->GetCameraTrans();

        return;
    }
    else {
        DirectX::XMFLOAT3X4 resetCamera;
        DirectX::XMStoreFloat3x4(&resetCamera, gameMatrix);
        g_mainHandle->GetCameraManager()->SetResetMatrix(resetCamera);

        return oUpdateView(pThis, frameTime, isActive);
        
    }


}

char __fastcall hookChangeResolution(CryLich::CD3D9Renderer* pThis, int nNewWidth, int nNewHeight, int nNewColDepth, int nNewRefreshHZ, bool bFullScreen, bool bForceReset) {

    //This hook is at the end of this function, in order to make sure the window style and position remain what we need
    //in order to do the hotsampling. Otherwise it will not be able to exceed the desktop size, and it will attempt to 
    //put the window in a position based on the monitor size, which we changed to a large value to accomodate any resolution.

    CryLich::ICVar* gameResW = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("r_width");
    CryLich::ICVar* gameResH = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("r_height");

    ::SetWindowLong(g_gameHwnd, -16, WS_BORDER | WS_CLIPSIBLINGS | WS_DLGFRAME | WS_MAXIMIZEBOX | WS_MINIMIZEBOX | WS_SYSMENU | WS_THICKFRAME | WS_VISIBLE);
    ::SetWindowLong(g_gameHwnd, -20, WS_EX_WINDOWEDGE);
    ::SetWindowPos(g_gameHwnd, HWND_TOP, 0, 0, g_mainHandle->GetCameraManager()->setScreen.x, g_mainHandle->GetCameraManager()->setScreen.y, SWP_NOOWNERZORDER | SWP_NOSENDCHANGING | SWP_NOZORDER | SWP_NOACTIVATE);

    return oChangeResolution(pThis, nNewWidth, nNewHeight, nNewColDepth, nNewRefreshHZ, bFullScreen, bForceReset);
}



void __fastcall hookUpdateKeyboard(CryLich::CKeyboard* pThis, bool bFocus) {

    if (g_mainHandle->GetCameraManager()->IsCameraEnabled() ||
        g_mainHandle->GetUI()->IsEnabled())
        return;

    return oUpdateKeyboard(pThis, bFocus);
}

void __fastcall hookUpdateMouse(CryLich::CMouse* pThis, bool bFocus, float a3) {

    if (g_mainHandle->GetCameraManager()->IsCameraEnabled() ||
        g_mainHandle->GetUI()->IsEnabled())
        return;

    return oUpdateMouse(pThis, bFocus, a3);

}

void __fastcall hookUpdateGamepad(CryLich::CXInputDevice* pThis, bool bFocus) {

    if (g_mainHandle->GetCameraManager()->IsCameraEnabled() ||
        g_mainHandle->GetUI()->IsEnabled())
        return;

    return oUpdateGamepad(pThis, bFocus);
}

BOOL WINAPI hSetCursorPos(int x, int y)
{
    if (g_mainHandle->GetUI()->IsEnabled())
        return TRUE;

    return TRUE;
}

bool __fastcall hookOnInputEvent(CryLich::CHardwareMouse* pThis, const CryLich::SInputEvent* rInputEvent) {

    //Trying to figure out why the cursor disappears when this tool is injected. This didn't help. 

    g_mainHandle->GetCameraManager()->cryMouse = pThis;

    g_mainHandle->GetCameraManager()->cryMouse->m_bFocus = true;

    return oOnInputEvent(pThis, rInputEvent);
}

namespace
{
    std::unordered_map<std::string, util::hooks::Hook> m_CreatedHooks;
}

// Write to VTable
static PBYTE WINAPI WriteToVTable(PDWORD64* ppVTable, PVOID hook, SIZE_T iIndex)
{
    DWORD dwOld = 0;
    VirtualProtect((void*)((*ppVTable) + iIndex), sizeof(PDWORD64), PAGE_EXECUTE_READWRITE, &dwOld);

    PBYTE pOrig = ((PBYTE)(*ppVTable)[iIndex]);
    (*ppVTable)[iIndex] = (DWORD64)hook;

    VirtualProtect((void*)((*ppVTable) + iIndex), sizeof(PDWORD64), dwOld, &dwOld);
    return pOrig;
}

template <typename T>
static void CreateVTableHook(std::string const& name, PDWORD64* ppVTable, PVOID hook, SIZE_T iIndex, T original)
{
    LPVOID* pOriginal = reinterpret_cast<LPVOID*>(original);
    *pOriginal = reinterpret_cast<LPVOID>(WriteToVTable(ppVTable, hook, iIndex));

    util::hooks::Hook hookInfo{ 0 };
    hookInfo.Address = (__int64)ppVTable;
    hookInfo.Index = iIndex;
    hookInfo.Type = util::hooks::HookType::VTable;
    hookInfo.Original = pOriginal;

    m_CreatedHooks.emplace(name, hookInfo);
}




void util::hooks::Init() {

    MH_STATUS status = MH_Initialize();
    if (status != MH_OK)
        util::log::Error("Failed to initialize MinHook, MH_STATUS 0x%X", status);
    

    __int64 UpdateView = (__int64)g_gameHandle + 0x2DA740;
    __int64 DevMode = (__int64)g_gameHandle + 0xEA56D0;
    __int64 UpdateKeyboard = (__int64)g_gameHandle + 0xAA2A10;
    __int64 UpdateMouse = (__int64)g_gameHandle + 0xAA3410;
    __int64 UpdateGamepad = (__int64)g_gameHandle + 0xAA7E50;
    __int64 SetFrustum = (__int64)g_gameHandle + 0xFE860;
    __int64 GameUpdate = (__int64)g_gameHandle + 0x114FB40;
    __int64 EngineUpdate = (__int64)g_gameHandle + 0x1208F0;
    __int64 UIUpdate = (__int64)g_gameHandle + 0x229220;
    __int64 SystemUpdate = (__int64)g_gameHandle + 0xEA86D0;
    __int64 SetViewport = (__int64)g_gameHandle + 0x1450C50;
    __int64 PreUpdate = (__int64)g_gameHandle + 0x5DD220;
    __int64 ChangeResolution = (__int64)g_gameHandle + 0x1589A03;
    __int64 OnInputEvent = (__int64)g_gameHandle + 0xE81FB0;

    MH_CreateHook((LPVOID)(DevMode), (LPVOID)(hookDevMode), NULL);
    MH_EnableHook((LPVOID)(DevMode));

    MH_CreateHook((LPVOID)(UpdateView), (LPVOID)(hookUpdateView), (LPVOID*)(&oUpdateView));
    MH_EnableHook((LPVOID)(UpdateView));

    MH_CreateHook((LPVOID)(UpdateKeyboard), (LPVOID)(hookUpdateKeyboard), (LPVOID*)(&oUpdateKeyboard));
    MH_EnableHook((LPVOID)(UpdateKeyboard));

    MH_CreateHook((LPVOID)(UpdateMouse), (LPVOID)(hookUpdateMouse), (LPVOID*)(&oUpdateMouse));
    MH_EnableHook((LPVOID)(UpdateMouse));

    MH_CreateHook((LPVOID)(UpdateGamepad), (LPVOID)(hookUpdateGamepad), (LPVOID*)(&oUpdateGamepad));
    MH_EnableHook((LPVOID)(UpdateGamepad));

    MH_CreateHook((LPVOID)(GameUpdate), (LPVOID)(hookGameUpdate), (LPVOID*)(&oGameUpdate));
    MH_EnableHook((LPVOID)(GameUpdate));

    MH_CreateHook((LPVOID)(EngineUpdate), (LPVOID)(hookUpdateEngine), (LPVOID*)(&oUpdateEngine));
    MH_EnableHook((LPVOID)(EngineUpdate));

    MH_CreateHook((LPVOID)(UIUpdate), (LPVOID)(hookUpdateUI), (LPVOID*)(&oUpdateUI));
    MH_EnableHook((LPVOID)(UIUpdate));

    MH_CreateHook((LPVOID)(SystemUpdate), (LPVOID)(hookSystemUpdate), (LPVOID*)(&oUpdateSystem));
    MH_EnableHook((LPVOID)(SystemUpdate));

    MH_CreateHook((LPVOID)(SetViewport), (LPVOID)(hookSetViewport), (LPVOID*)(&oSetViewport));
    MH_EnableHook((LPVOID)(SetViewport));

    MH_CreateHook((LPVOID)(PreUpdate), (LPVOID)(hookPreUpdate), (LPVOID*)(&oPreUpdate));
    MH_EnableHook((LPVOID)(PreUpdate));

    MH_CreateHook((LPVOID)(ChangeResolution), (LPVOID)(hookChangeResolution), (LPVOID*)(&oChangeResolution));
    MH_EnableHook((LPVOID)(ChangeResolution));

    MH_CreateHook((LPVOID)(OnInputEvent), (LPVOID)(hookOnInputEvent), (LPVOID*)(&oOnInputEvent));
    MH_EnableHook((LPVOID)(OnInputEvent));

    HMODULE hUser32 = GetModuleHandleA("user32.dll");
    FARPROC pSetCursorPos = GetProcAddress(hUser32, "SetCursorPos");
    MH_CreateHook((LPVOID)(pSetCursorPos), (LPVOID)(hSetCursorPos), NULL);
    MH_EnableHook((LPVOID)(pSetCursorPos));

    if (status != MH_OK) {
        util::log::Error("Some hooks failed to initialize!");
    }
    else {
        util::log::Ok("Hooks initialized.");
    }

    CreateVTableHook("SwapChainPresent", (PDWORD64*)g_dxgiSwapChain, hIDXGISwapChain_Present, 8, &oIDXGISwapChain_Present);

}


//I never implemented Hatti's hook functions, maybe later

void util::hooks::SetHookState(bool enable, std::string const& name) {
    /*if (name.empty())
    {
        MH_STATUS status = enable ? MH_EnableHook(MH_ALL_HOOKS) : MH_DisableHook(MH_ALL_HOOKS);
        if (status != MH_OK)
            util::log::Error("MinHook failed to %s all hooks, MH_STATUS 0x%X", (enable ? "enable" : "disable"), status);

        for (auto& entry : m_CreatedHooks)
        {
            Hook& hook = entry.second;
            if (hook.Type == HookType::MinHook) continue;
            if (hook.Enabled != enable)
            {
                *hook.Original = WriteToVTable((PDWORD64*)hook.Address, *hook.Original, hook.Index);
                hook.Enabled = enable;
            }
            else
                util::log::Warning("VTable hook %s is already %s", name.c_str(), enable ? "enabled" : "disabled");
        }
    }
    else
    {
        auto result = m_CreatedHooks.find(name);
        if (result == m_CreatedHooks.end())
        {
            util::log::Error("Hook %s does not exit", name.c_str());
            return;
        }

        Hook& hook = result->second;
        if (hook.Type == HookType::MinHook)
        {
            MH_STATUS status = enable ? MH_EnableHook((LPVOID)hook.Address) : MH_DisableHook((LPVOID)hook.Address);
            if (status != MH_OK)
                util::log::Error("MinHook failed to %s hook %s, MH_STATUS 0x%X", (enable ? "enable" : "disable"), name.c_str(), status);
        }
        else
        {
            if (hook.Enabled != enable)
            {
                *hook.Original = WriteToVTable((PDWORD64*)hook.Address, *hook.Original, hook.Index);
                hook.Enabled = enable;
            }
            else
                util::log::Warning("VTable hook %s is already %s", name.c_str(), enable ? "enabled" : "disabled");
        }
    }*/
}