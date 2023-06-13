#define NOMINMAX

#define DISTANCE_TO_THE_SUN 1000000

#include <Windows.h>

#include "CryLich.h"
#include "CameraManager.h"
#include "Main.h"
#include "Util.h"
#include "InputSystem.h"
#include "INIReader.h"
#include "UI.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "ImGuiEXT.h"
#include "SimpleMath.h"
#include <algorithm>
#include <vector>
#include <math.h>

extern CryLich::SCVars* g_pGameCVars;
extern HWND g_gameHwnd;

using namespace DirectX;

CameraManager::CameraManager() :
    m_CameraEnabled(false),
    m_FirstEnable(true),
    m_GamepadDisabled(true),
    m_KbmDisabled(true),
    m_TrackPlayer(),
    m_AutoReset(true),
    m_OverrideAspectRatio(false),
    m_AspectRatio(1.777777f),
    m_pitch(0.0),
    m_yaw(0.0),
    m_roll(0.0)
{

}

CameraManager::~CameraManager()
{
    
}

void CameraManager::HotkeyUpdate()
{
    InputSystem* pInput = g_mainHandle->GetInputSystem();

    if (pInput->IsActionDown(Action::ToggleCamera))
    {
        ToggleCamera();
        while (pInput->IsActionDown(Action::ToggleCamera))
            Sleep(1);
    }

    if (pInput->IsActionDown(Action::ToggleHUD))
    {
        ToggleHUD();

        if (!m_HUDEnabled) {

            cryUI->m_bHudVisible = 1;
        }
        else {

            cryUI->m_bHudVisible = 0;
        }
        while (pInput->IsActionDown(Action::ToggleHUD))
            Sleep(1);
    }

    if (pInput->IsActionDown(Action::ToggleFreezeTime))
    {
        m_TimeFreezeEnabled = !m_TimeFreezeEnabled;

        //This seems to be the only way to get a timestop without also being unable to 
        //move the camera. I tried several other things, everything else gives you the full
        //menu pause.

        CryLich::ICVar* pause = cryConsole->GetCVar("t_Scale");
        
        if (m_TimeFreezeEnabled) {
            pause->Set(0);
        }
        else {
            pause->Set(1);
        }

        while (pInput->IsActionDown(Action::ToggleFreezeTime))
            Sleep(1);
    }

    if (m_CameraEnabled)
    {
        //I haven't implemented the tracks yet. Probably won't for this game.

        if (pInput->IsActionDown(Action::Track_CreateNode))
        {
            m_TrackPlayer.CreateNode(m_Camera);

            while (pInput->IsActionDown(Action::Track_CreateNode))
                Sleep(1);
        }

        if (pInput->IsActionDown(Action::Track_DeleteNode))
        {
            m_TrackPlayer.DeleteNode();

            while (pInput->IsActionDown(Action::Track_DeleteNode))
                Sleep(1);
        }

        if (pInput->IsActionDown(Action::Track_Play))
        {
            m_TrackPlayer.Toggle();

            while (pInput->IsActionDown(Action::Track_Play))
                Sleep(1);
        }
    }
}


void CameraManager::Update(double dt)
{
    if (!m_CameraEnabled) return;

    if (m_UiRequestReset)
    {
        m_UiRequestReset = false;
        ResetCamera();
    }

    UpdateInput(dt);
    UpdateCamera(dt);
}

void CameraManager::DrawUI()
{
    ImGuiIO& io = ImGui::GetIO();

    ImGui::Dummy(ImVec2(0, 10));
    ImGui::Dummy(ImVec2(300, 0));

    ImGui::PushStyleColor(ImGuiCol_Border, ImVec4(1, 1, 1, 1));
    ImGui::PushStyleVar(ImGuiStyleVar_FrameBorderSize, 1.f);
    ImGui::PushFont(io.Fonts->Fonts[3]);
    ImGui::SameLine();
    if (ImGui::ToggleButton(m_CameraEnabled ? "Disable" : "Enable", ImVec2(100, 25), m_CameraEnabled, true))
        ToggleCamera();
    ImGui::SameLine();
    m_UiRequestReset |= ImGui::Button("Reset", ImVec2(100, 25));

    ImGui::PopFont();
    ImGui::PopStyleColor();
    ImGui::PopStyleVar();
    ImGui::PushFont(io.Fonts->Fonts[4]);

    ImGui::Dummy(ImVec2(0, 0));
    ImGui::Columns(3, 0, false);
    ImGui::NextColumn();
    ImGui::SetColumnOffset(-1, 12);

    ImGui::PushItemWidth(200);
    bool configChanged = false;

    ImGui::Text("Movement speed");
    configChanged |= ImGui::InputFloat("##CameraMovementSpeed", &m_Camera.MovementSpeed, 0.1f, 1.0f, "%.3f", 2);
    ImGui::Text("Rotation speed");
    configChanged |= ImGui::InputFloat("##CameraRotationSpeed", &m_Camera.RotationSpeed, 0.1f, 1.0f, "%.3f", 2);
    ImGui::Text("Roll speed");
    configChanged |= ImGui::InputFloat("##CameraRollSpeed", &m_Camera.RollSpeed, 0.1f, 1.0f, "%.3f", 2);
    ImGui::Text("FoV speed");
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 10));
    configChanged |= ImGui::InputFloat("##CameraFoVSpeed", &m_Camera.FovSpeed, 0.1f, 1.0f, "%.3f", 2);

    if (configChanged)
        g_mainHandle->OnConfigChanged();

    ImGui::Checkbox("Reset camera automatically", &m_AutoReset);
    ImGui::PopStyleVar();

    ImGui::NextColumn();
    ImGui::SetColumnOffset(-1, 290);
    ImGui::PushItemWidth(200);

    ImGui::Text("Field of view");
    ImGui::InputFloat("##CameraFoV", &m_Camera.FieldOfView, 1.f, 1.f, "%.3f", 2);

    m_TrackPlayer.DrawUI();
    ImGui::PopFont();
}

void CameraManager::ReadConfig(INIReader* pReader)
{
    m_Camera.MovementSpeed = pReader->GetReal("Camera", "MovementSpeed", 1.0f);
    m_Camera.RotationSpeed = pReader->GetReal("Camera", "RotationSpeed", XM_PI / 4);
    m_Camera.RollSpeed = pReader->GetReal("Camera", "RollSpeed", XM_PI / 8);
    m_Camera.FovSpeed = pReader->GetReal("Camera", "FovSpeed", 5.0f); 
}

const std::string CameraManager::GetConfig()
{
    std::string config = "[Camera]\n";
    config += "MovementSpeed = " + std::to_string(m_Camera.MovementSpeed) + "\n";
    config += "RotationSpeed = " + std::to_string(m_Camera.RotationSpeed) + "\n";
    config += "RollSpeed = " + std::to_string(m_Camera.RollSpeed) + "\n";
    config += "FovSpeed = " + std::to_string(m_Camera.FovSpeed) + "\n";

    return config;
}


void CameraManager::UpdateCamera(double dt)
{
    //CryEngine is Z up, Y into the screen, and I find all this very confusing but here is where I altered the 
    //coordinates to be calculated correctly within this function. I switched the Yaw and Roll here.
    XMVECTOR qPitch = DirectX::XMQuaternionRotationRollPitchYaw(-m_Camera.dPitch * dt * m_Camera.RotationSpeed, 0, 0);
    XMVECTOR qYaw = DirectX::XMQuaternionRotationRollPitchYaw(0, 0 , -m_Camera.dYaw * dt * m_Camera.RotationSpeed);
    XMVECTOR qRoll = DirectX::XMQuaternionRotationRollPitchYaw(0, -m_Camera.dRoll * dt * m_Camera.RollSpeed, 0);

    XMVECTOR qRotation = DirectX::XMLoadFloat4(&m_Camera.Rotation);
    XMVECTOR vPosition = DirectX::XMLoadFloat3(&m_Camera.Position);

    // Hatti: Adds delta rotations to camera rotation
    qRotation = DirectX::XMQuaternionMultiply(qPitch, qRotation);
    qRotation = DirectX::XMQuaternionMultiply(qRotation, qYaw);
    qRotation = DirectX::XMQuaternionMultiply(qRoll, qRotation);

    // Hatti: Make sure it's normalized
    qRotation = DirectX::XMQuaternionNormalize(qRotation);

    m_Camera.FieldOfView += m_Camera.dFov * dt * m_Camera.FovSpeed;

    // Hatti: If a camera track is being played, get the current
    // state and overwrite position/rotation/FoV.
    if (m_TrackPlayer.IsPlaying())
    {
        CatmullRomNode resultNode = m_TrackPlayer.PlayForward(dt);

        if (m_TrackPlayer.IsRotationLocked())
            qRotation = DirectX::XMLoadFloat4(&resultNode.Rotation);

        if (m_TrackPlayer.IsFovLocked())
            m_Camera.FieldOfView = resultNode.FieldOfView;

        vPosition = DirectX::XMLoadFloat3(&resultNode.Position);
    }
    // Hatti: Create rotation matrix
    XMMATRIX rotMatrix = DirectX::XMMatrixRotationQuaternion(qRotation);

    // Hatti: Add delta positions
    if (!m_TrackPlayer.IsPlaying())
    {
        vPosition += m_Camera.dX * rotMatrix.r[0] * dt * m_Camera.MovementSpeed;
        vPosition += m_Camera.dY * rotMatrix.r[1] * dt * m_Camera.MovementSpeed;
        vPosition += m_Camera.dZ * rotMatrix.r[2] * dt * m_Camera.MovementSpeed;
    }

    // Hatti: Store results
    rotMatrix.r[3] = vPosition;
    rotMatrix.r[3].m128_f32[3] = 1.0f;

    m_Camera.Matrix = rotMatrix;
    DirectX::XMStoreFloat3(&m_Camera.Position, vPosition);
    DirectX::XMStoreFloat4(&m_Camera.Rotation, qRotation);

    DirectX::XMStoreFloat3x4(&m_Camera.Transform, rotMatrix);

    //This was the easiest way to make sure I had the matrix oriented right
    m_gameMatrixLocation->_11 = m_Camera.Transform._11;
    m_gameMatrixLocation->_12 = m_Camera.Transform._12;
    m_gameMatrixLocation->_13 = m_Camera.Transform._13;
    m_gameMatrixLocation->_14 = m_Camera.Transform._14;
    m_gameMatrixLocation->_21 = m_Camera.Transform._21;
    m_gameMatrixLocation->_22 = m_Camera.Transform._22;
    m_gameMatrixLocation->_23 = m_Camera.Transform._23;
    m_gameMatrixLocation->_24 = m_Camera.Transform._24;
    m_gameMatrixLocation->_31 = m_Camera.Transform._31;
    m_gameMatrixLocation->_32 = m_Camera.Transform._32;
    m_gameMatrixLocation->_33 = m_Camera.Transform._33;
    m_gameMatrixLocation->_34 = m_Camera.Transform._34;

    //This function had to be called to set the FOV
    CryLich::CCamera::SetFrustumSmartFOV(cryCamera, g_pGameCVars->cl_fovmaint, cryCamera->m_Width, cryCamera->m_Height, DirectX::XMConvertToRadians(m_Camera.FieldOfView), 
        g_pGameCVars->cl_fov_aspect_ratio, cryView->m_viewParams.nearplane, cry3DEngine->m_fMaxViewDistHighSpec, cryCamera->m_PixelAspectRatio);

    

    m_Camera.dX = 0;
    m_Camera.dY = 0;
    m_Camera.dZ = 0;
    m_Camera.dPitch = 0;
    m_Camera.dYaw = 0;
    m_Camera.dRoll = 0;
    m_Camera.dFov = 0;
}

void CameraManager::UpdateInput(double dt)
{
    InputSystem* pInput = g_mainHandle->GetInputSystem();
    if (!g_hasFocus || g_mainHandle->GetUI()->HasKeyboardFocus())
        return;



    m_Camera.dX = pInput->GetActionState(Camera_Right) - pInput->GetActionState(Camera_Left);
    m_Camera.dY = pInput->GetActionState(Camera_Forward) - pInput->GetActionState(Camera_Backward); 
    m_Camera.dZ = pInput->GetActionState(Camera_Up) - pInput->GetActionState(Camera_Down);


    m_Camera.dPitch = pInput->GetActionState(Camera_PitchDown) - pInput->GetActionState(Camera_PitchUp);
    m_Camera.dYaw = pInput->GetActionState(Camera_YawRight) - pInput->GetActionState(Camera_YawLeft);
    m_Camera.dRoll = pInput->GetActionState(Camera_RollLeft) - pInput->GetActionState(Camera_RollRight); 
    m_Camera.dFov = pInput->GetActionState(Camera_IncFov) - pInput->GetActionState(Camera_DecFov);

    if (m_KbmDisabled && !g_mainHandle->GetUI()->IsEnabled())
    {
        XMFLOAT3 mouseState = pInput->GetMouseState();
        m_Camera.dPitch += mouseState.y * dt;
        m_Camera.dYaw += mouseState.x * dt;
        m_Camera.dFov += mouseState.z * dt;
    }
}

void CameraManager::UpdateSunDir(ImVec2 mousePos, double dt)
{
    //This function moves the sun along the skysphere using the good old Pythagorean Theorem.
    //This is possible and quite simple because the skysphere is a sphere with a fixed, known radius.
    //If we imagine the XY plane in the game to be the equivalent of the XY plane of the screen,
    //we can translate this easily to a point along the surface of this fixed sphere, so all the
    //user has to do is move the mouse to move the sun position in an intuitive way.

    float maxX = DISTANCE_TO_THE_SUN;
    float maxY = DISTANCE_TO_THE_SUN;
    float rSky = DISTANCE_TO_THE_SUN;

    ImVec2 mousePosCurrent = ImGui::GetIO().MousePos;

    RECT gameWindow;
    ::GetWindowRect(g_gameHwnd, &gameWindow);
    int gameWidthExtra = util::FindExtraWindowWidth(g_gameHwnd);
    int gameHeightExtra = util::FindExtraWindowHeight(g_gameHwnd);

    float screenX = (float)(gameWindow.right - gameWindow.left)-(float)(gameWidthExtra);
    float screenY = (float)(gameWindow.bottom - gameWindow.top)-(float)(gameHeightExtra);

    float center[2] = { screenX / 2, screenY / 2 };

    float mouseDistCenter[2] = { mousePosCurrent.x - center[0] , mousePosCurrent.y - center[1] };

    float worldX = (mouseDistCenter[0] * DISTANCE_TO_THE_SUN) / screenX;
    float worldY = (mouseDistCenter[1] * DISTANCE_TO_THE_SUN) / screenY;

    if (worldX > DISTANCE_TO_THE_SUN || worldY > DISTANCE_TO_THE_SUN || worldX < -DISTANCE_TO_THE_SUN || worldY < -DISTANCE_TO_THE_SUN) {

        return;
    }

    float rPlane = ::sqrt((worldX * worldX) + (worldY * worldY));

    float worldZ = ::sqrt((rSky * rSky) - (rPlane * rPlane));

    cry3DEngine->m_vSunDir[0] = worldX;
    cry3DEngine->m_vSunDir[1] = worldY;
    cry3DEngine->m_vSunDir[2] = worldZ;

    DirectX::XMVECTOR sunVec = { worldX, worldY, worldZ };

    sunVec = XMVector3Normalize(sunVec);

    //This vector also appears in the UI
    cry3DEngine->m_vSunDirNormalized[0] = sunVec.m128_f32[0];
    cry3DEngine->m_vSunDirNormalized[1] = sunVec.m128_f32[1];
    cry3DEngine->m_vSunDirNormalized[2] = sunVec.m128_f32[2];
    


    

}

void CameraManager::UpdateResolution(int x, int y)
{
    //This was an old version of this function, for when it was called differently

    CryLich::ICVar* gameResW = cryConsole->GetCVar("r_width");
    CryLich::ICVar* gameResH = cryConsole->GetCVar("r_height");

    cryD3D9Renderer->m_prefMonX = 0;
    cryD3D9Renderer->m_prefMonY = 0;

    gameResW->Set(x);
    gameResH->Set(y);

    ::SetWindowPos(g_gameHwnd, HWND_TOP, 0, 0, gameResW->GetIVal(), gameResH->GetIVal(), SWP_SHOWWINDOW | SWP_NOSIZE);
}

void CameraManager::ChangeResolution()
{
    //This takes the window resolution and uses it to set the resolution of the game using CVars

    RECT gameWindow;
    RECT gameWindowFull;
    ::GetClientRect(g_gameHwnd, &gameWindow);
    ::GetWindowRect(g_gameHwnd, &gameWindowFull);

    setScreen.x = gameWindowFull.right - gameWindowFull.left;
    setScreen.y = gameWindowFull.bottom - gameWindowFull.top;

    int gameWidth = gameWindow.right;
    int gameHeight = gameWindow.bottom;

    CryLich::ICVar* gameResW = cryConsole->GetCVar("r_width");
    CryLich::ICVar* gameResH = cryConsole->GetCVar("r_height");

    gameResW->Set(gameWidth);
    gameResH->Set(gameHeight);

    //Because when the camera is enabled, a whole spate of things don't get called automatically, including this
    if (m_CameraEnabled) {
        CryLich::CCamera::SetFrustumSmartFOV(cryCamera, g_pGameCVars->cl_fovmaint, gameWidth, gameHeight, DirectX::XMConvertToRadians(m_Camera.FieldOfView),
            g_pGameCVars->cl_fov_aspect_ratio, cryView->m_viewParams.nearplane, cry3DEngine->m_fMaxViewDistHighSpec, cryCamera->m_PixelAspectRatio);

        CryLich::CCamera::UpdateFrustum(cryCamera);
    }

}

CameraManager::m_screen CameraManager::FindScreenSize()
{
    //Not being used currently, but it is handy

    CryLich::ICVar* gameResW = cryConsole->GetCVar("r_width");
    CryLich::ICVar* gameResH = cryConsole->GetCVar("r_height");

    int gameWidth = gameResW->GetIVal();
    int gameHeight = gameResH->GetIVal();

    m_screen screenTemp = { gameWidth, gameHeight };

    return screenTemp;
}



void CameraManager::ToggleCamera()
{
    // If first enable, fetch game camera location
    if (m_FirstEnable || m_AutoReset)
    {
        m_Camera.Rotation = XMFLOAT4(0, 0, 0, 1);
        m_Camera.Position = XMFLOAT3(m_ResetMatrix.m[0][3], m_ResetMatrix.m[1][3], m_ResetMatrix.m[2][3]);
        m_Camera.Matrix.r[3] = DirectX::XMLoadFloat3(&m_Camera.Position);
        m_FirstEnable = false;
    }

    m_CameraEnabled = !m_CameraEnabled;

    if (m_CameraEnabled) {

        //This prevents the hands in first person from showing when the camera is toggled
        CryLich::ICVar* noHands = cryConsole->GetCVar("r_drawnearfov");
        noHands->Set(2);
    }

    if (!m_CameraEnabled) {

        CryLich::ICVar* noHands = cryConsole->GetCVar("r_drawnearfov");
        noHands->Set(90);
    }
}

void CameraManager::ToggleHUD() {

    m_HUDEnabled = !m_HUDEnabled;
}

void CameraManager::ResetCamera()
{
    // There's a few ways you can do this
    // Best way might depend on the game

    // In BF tools camera is toggled off, then Sleep() for a few
    // 100 ms, fetch the game camera location again and reset to
    // that and then re-enable camera again.

    if (!m_CameraEnabled) return;
    m_CameraEnabled = false;
    Sleep(100);

    m_Camera.Position = XMFLOAT3(&m_ResetMatrix.m[3][0]);
    m_Camera.Rotation = XMFLOAT4(0, 0, 0, 1);

    m_CameraEnabled = true;
}