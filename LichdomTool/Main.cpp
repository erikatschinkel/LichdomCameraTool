#include "Main.h"
#include "Util.h"
#include "InputSystem.h"
#include "CameraManager.h"
#include "INIReader.h"
#include "CryLich.h"
#include "UI.h"
#include "MinHook.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

#include <d3d11.h>
#include <fstream>
#include <boost/filesystem.hpp>
#include <boost/chrono.hpp>

static const char* g_gameName = "Lichdom: Battlemage";
static const char* g_moduleName = "LichdomBattlemage.exe";
static const char* g_className = "CryENGINE";
static const char* g_windowTitle = "Battlemage - Build (2014.12.08.70569-Release_v1_2_x64)";
static const char* g_configFile = "./Cinematic Tools/config.ini";

HINSTANCE g_gameHandle = NULL;
HINSTANCE g_dllHandle = NULL;
HWND g_gameHwnd = NULL;
WNDPROC g_origWndProc = 0;
Main* g_mainHandle = nullptr;

CryLich::SCVars* g_pGameCVars;

ID3D11DeviceContext* g_d3d11Context = nullptr;
ID3D11Device* g_d3d11Device = nullptr;
IDXGISwapChain* g_dxgiSwapChain = nullptr;

bool g_shutdown = false;
bool g_hasFocus = false;

Main::Main():
    m_Initialized(false),
    m_ConfigChanged(false),
    m_dtConfigCheck(0)
{

}

Main::~Main()
{
}

bool Main::Initialize() {
    boost::filesystem::path dir("Cinematic Tools");
    if (!boost::filesystem::exists(dir))
        boost::filesystem::create_directory(dir);

    util::log::Init();
    util::log::Write("Cinematic Tools for %s\n", g_gameName);

    // Hatti: Needed for ImGui + other functionality
    g_gameHwnd = FindWindowA(g_className, NULL);
    if (g_gameHwnd == NULL)
    {
        g_gameHwnd = FindWindowA(NULL, g_windowTitle);
        if (g_gameHwnd == NULL)
        {
            util::log::Error("Failed to retrieve window handle, GetLastError 0x%X", GetLastError());
            return false;
        }
    }

    g_gameHandle = ::GetModuleHandleA("LichdomBattlemage.exe");
    if (g_gameHandle == NULL)
    {
        util::log::Error("Failed to retrieve module handle, GetLastError 0x%X", GetLastError());
        return false;
    }

    g_d3d11Device = CryLich::CD3D9Renderer::GetDevice();
    g_d3d11Context = CryLich::CD3D9Renderer::GetContext();
    g_dxgiSwapChain = CryLich::CD3D9Renderer::GetSwapChain();

    if (!g_d3d11Context || !g_d3d11Device || !g_dxgiSwapChain)
    {
        util::log::Error("Failed to retrieve Dx11 interfaces");
        util::log::Error("Device 0x%I64X DeviceContext 0x%I64X SwapChain 0x%I64X", g_d3d11Device, g_d3d11Context, g_dxgiSwapChain);
        return false;
    }
    else {
        util::log::Write("DirectX interfaces hooked!");
    }

    m_pCameraManager = std::make_unique<CameraManager>();
    m_pInputSystem = std::make_unique<InputSystem>();
    m_pUI = std::make_unique<UI>();

    m_pInputSystem->Initialize();
    if (!m_pUI->Initialize())
        return false;

    util::hooks::Init();
    
    LoadConfig();

    g_origWndProc = (WNDPROC)SetWindowLongPtr(g_gameHwnd, -4, (LONG_PTR)&WndProc);
    if (g_origWndProc == 0)
    {
        util::log::Error("Failed to set WndProc, GetLastError 0x%X", GetLastError());
        return false;
    }

    return true;

}

void Main::Run()
{
    // Hatti: Main update loop
    boost::chrono::high_resolution_clock::time_point lastUpdate = boost::chrono::high_resolution_clock::now();
    RECT gameWindow;
    RECT gameWindowFull;
    CryLich::ICVar* gameResW = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("r_width");
    CryLich::ICVar* gameResH = g_mainHandle->GetCameraManager()->cryConsole->GetCVar("r_height");

    while (!g_shutdown)
    {
        boost::chrono::duration<double> dt = boost::chrono::high_resolution_clock::now() - lastUpdate;
        lastUpdate = boost::chrono::high_resolution_clock::now();

        m_pCameraManager->Update(dt.count());
        m_pUI->Update(dt.count());

        ::GetClientRect(g_gameHwnd, &gameWindow);
        ::GetWindowRect(g_gameHwnd, &gameWindowFull);

        //This will emulate hotsampling, by detecting changes to the window size and calling this ChangeResolution function

        if (WM_SIZE && WM_EXITSIZEMOVE && (gameWindow.right != gameResW->GetIVal() || gameWindow.bottom != gameResH->GetIVal()) && (gameWindowFull.left < g_mainHandle->GetCameraManager()->cryD3D9Renderer->m_deskwidth && gameWindow.top < g_mainHandle->GetCameraManager()->cryD3D9Renderer->m_deskheight)) {
            m_pCameraManager->ChangeResolution();
        }

        // Hatti: Check if config has been affected, if so, save it
        m_dtConfigCheck += dt.count();
        if (m_dtConfigCheck > 10.f)
        {
            m_dtConfigCheck = 0;
            if (m_ConfigChanged)
            {
                m_ConfigChanged = false;
                SaveConfig();
            }
        }

        Sleep(10);
    }

    // Hatti: Save config and disable hooks before exit
    SaveConfig();
    util::hooks::SetHookState(false);
}

void Main::LoadConfig()
{
    // Hatti: Read config.ini using inih by Ben Hoyt
    // https://github.com/benhoyt/inih

    m_pConfig = std::make_unique<INIReader>(g_configFile);
    int parseResult = m_pConfig->ParseError();

    // Hatti: If there's problems reading the file, notify the user.
    // Code-wise it should be safe to just continue,
    // since you can still request variables from INIReader.
    // They'll just return the specified default value.
    if (parseResult != 0)
        util::log::Warning("Config file could not be loaded, using default settings");

    m_pCameraManager->ReadConfig(m_pConfig.get());
    m_pInputSystem->ReadConfig(m_pConfig.get());
}

void Main::SaveConfig()
{
    std::fstream file;
    file.open(g_configFile, std::ios_base::out | std::ios_base::trunc);

    if (!file.is_open())
    {
        util::log::Error("Could not save config, failed to open file for writing. GetLastError 0x%X", GetLastError());
        return;
    }

    file << m_pCameraManager->GetConfig();
    file << m_pInputSystem->GetConfig();

    file.close();
}

extern IMGUI_IMPL_API LRESULT ImGui_ImplWin32_WndProcHandler(HWND hWnd, UINT msg, WPARAM wParam, LPARAM lParam);
LRESULT CALLBACK Main::WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    if (ImGui_ImplWin32_WndProcHandler(hwnd, uMsg, wParam, lParam))
         return TRUE;

    switch (uMsg)
    {
    case WM_ACTIVATE:
    {
        g_hasFocus = (wParam != WA_INACTIVE);
        break;
    }
    case WM_INPUT:
    {
        if (!g_mainHandle->GetUI()->IsEnabled())
            g_mainHandle->GetInputSystem()->HandleRawInput(lParam);
        else
        return TRUE;
        break;
    }
    case WM_KEYDOWN:
    {
        if (g_mainHandle->GetInputSystem()->HandleKeyMsg(wParam, lParam))
            return TRUE;
        break;
    }
    case WM_MOUSEMOVE: case WM_MOUSEWHEEL:
    {
        g_mainHandle->GetInputSystem()->HandleMouseMsg(uMsg, wParam, lParam);

        if (g_mainHandle->m_pUI->IsEnabled())
            return TRUE;

        break;
    }
    case WM_SIZE:
    {
        // Hatti: Resize event
        g_mainHandle->m_pUI->OnResize();
        break;
    }
    case WM_DESTROY:
    {
        g_shutdown = true;
        break;
    }
    }

    return CallWindowProc(g_origWndProc, hwnd, uMsg, wParam, lParam);
}