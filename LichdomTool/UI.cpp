#include "UI.h"
#include "Main.h"
#include "Util.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include "CryLich.h"
#include "resource.h"
#include "ImGuiEXT.h"
#include <vector>
#include "ActionDefs.h"
#include <boost/chrono.hpp>

#include <WICTextureLoader.h>
#pragma comment(lib, "DirectXTK.lib")

const char* g_creditsText = "The following libraries are used in making the Cinematic Tools:\n"
"MinHook by Tsuda Kageyu, inih by Ben Hoyt, ImGui by Omar Cornut, C++ libraries by Boost, DirectXTK and DirectXTex by Microsoft.\n\n"
"And thank you, Hattiwatti for this great framework!";

UI::UI() :
    m_Enabled(false),
    m_FramesToSkip(0),
    m_HasKeyboardFocus(false),
    m_HasMouseFocus(false),
    m_IsResizing(false),
    m_pRTV(nullptr)
{

}

UI::~UI()
{
    ImGui_ImplDX11_Shutdown();
    ImGui::DestroyContext();
}

bool UI::Initialize()
{
    /////////////////////////
    // ImGui Configuration //
    /////////////////////////

    // ImGui is an immensely useful and simple-to-use UI system made by Omar Cornut.
    // Definitely check it out:
    // https://github.com/ocornut/imgui
    // 
    
    // Hatti: Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls
    io.Fonts->AddFontDefault();
    io.MouseDrawCursor = true;

    // Hatti: Setup Dear ImGui style
    ImGuiStyle& Style = ImGui::GetStyle();
    Style.WindowRounding = 0.0f;
    Style.ChildRounding = 0.0f;
    Style.WindowPadding = ImVec2(0, 0);
    Style.Colors[ImGuiCol_TitleBg] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    Style.Colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    Style.Colors[ImGuiCol_TitleBgActive] = ImVec4(0.10f, 0.09f, 0.12f, 1.00f);
    Style.Colors[ImGuiCol_Button] = ImVec4(.2f, .2f, .2f, 1.0f);
    Style.Colors[ImGuiCol_ButtonHovered] = ImVec4(0.26f, 0.26f, 0.26f, 1.0f);
    Style.Colors[ImGuiCol_ButtonActive] = ImVec4(1, 1, 1, 1);
    Style.Colors[ImGuiCol_ChildBg] = ImVec4(0, 0, 0, 0.7f);
    Style.Colors[ImGuiCol_Border] = ImVec4(0, 0, 0, 0);
    Style.Colors[ImGuiCol_BorderShadow] = ImVec4(0, 0, 0, 0);

    // Hatti: Setup Platform/Renderer backends
    ImGui_ImplWin32_Init(g_gameHwnd);
    ImGui_ImplDX11_Init(g_d3d11Device, g_d3d11Context);

    if (!ImGui_ImplDX11_Init(g_d3d11Device, g_d3d11Context) || !ImGui_ImplWin32_Init(g_gameHwnd))
    {
        util::log::Error("ImGui Dx11 implementation failed to initialize");
        return false;
    }

    if (!CreateRenderTarget()) {
        util::log::Error("Cannot create render target");
        return false;
    }

    ImFontConfig fontConfig;
    fontConfig.OversampleH = 8;
    fontConfig.OversampleV = 8;
    fontConfig.FontDataOwnedByAtlas = false;

    ImFontConfig fontOffsetConfig = fontConfig;
    fontOffsetConfig.GlyphOffset = ImVec2(0, -2);

    void* pData;
    DWORD szData;

    if (!util::GetResource(IDR_FONT_PURISTA, pData, szData))
    {
        util::log::Error("Failed to load IDR_FONT_PURISTASEMI from resources");
        return false;
    }

    io.Fonts->AddFontFromMemoryTTF(pData, szData, 24, &fontConfig);
    io.Fonts->AddFontFromMemoryTTF(pData, szData, 20, &fontOffsetConfig);
    io.Fonts->AddFontFromMemoryTTF(pData, szData, 18, &fontOffsetConfig);

    if (!util::GetResource(IDR_FONT_SEGOE, pData, szData))
    {
        util::log::Error("Failed to load IDR_FONT_SEGOEUI from resources");
        return false;
    }

    io.Fonts->AddFontFromMemoryTTF(pData, szData, 16, &fontConfig);
    io.Fonts->AddFontFromMemoryTTF(pData, szData, 20, &fontConfig);


    m_TitleImage = CreateImageFromResource(IDR_IMG_TITLE);
    for (int i = IDR_IMG_BG1; i <= IDR_IMG_BG1; ++i)
        m_BgImages.emplace_back(CreateImageFromResource(i));

    util::log::Ok("UI Initialized");
    return true;
}

void UI::Draw()
{
    if (!m_Enabled) return;
    if (m_IsResizing)
    {
        // Hatti: I think it's a good idea to skip some frames after a resize
        // juuuuust to make sure the game's done with resizing stuff.

        m_FramesToSkip -= 1;
        if (m_FramesToSkip == 0)
        {
            m_IsResizing = false;
            CreateRenderTarget();
        }
        return;
    }
    boost::chrono::high_resolution_clock::time_point lastUpdate = boost::chrono::high_resolution_clock::now();

    ImGuiIO& io = ImGui::GetIO();
    io.Fonts->AddFontDefault();
    g_d3d11Context->OMSetRenderTargets(1, m_pRTV.GetAddressOf(), nullptr);
    ImGui_ImplDX11_NewFrame();
    ImGui_ImplWin32_NewFrame();
    ImGui::NewFrame();
    ImGui::SetNextWindowSize(ImVec2(800, 460));
    ImGui::Begin("Cinematic Tools", nullptr, ImGuiWindowFlags_NoResize);
    {
        ImVec2 windowPos = ImGui::GetWindowPos();
        ImGui::Dummy(ImVec2(0, 20));

        ImGui::GetWindowDrawList()->AddImage(m_BgImages[0].pSRV.Get(), ImVec2(windowPos.x, windowPos.y + 19), ImVec2(windowPos.x + 800, windowPos.y + 460));
        //if (m_isFading)
        //  ImGui::GetWindowDrawList()->AddImage(m_bgImages[m_nextIndex].pShaderResourceView.Get(), ImVec2(windowPos.x, windowPos.y + 19), ImVec2(windowPos.x + 800, windowPos.y + 460),
        //    ImVec2(0, 0), ImVec2(1, 1), ImGui::ColorConvertFloat4ToU32(ImVec4(1, 1, 1, m_opacity)));

        ImGui::GetWindowDrawList()->AddRectFilled(ImVec2(windowPos.x, windowPos.y + 19), ImVec2(windowPos.x + 800, windowPos.y + 460), 0x10000000);
        ImGui::GetWindowDrawList()->AddImage(m_TitleImage.pSRV.Get(), ImVec2(windowPos.x + 210, windowPos.y + 85), ImVec2(windowPos.x + 592, windowPos.y + 131));

        ImGui::PushFont(io.Fonts->Fonts[2]);
        ImGui::Dummy(ImVec2(143, 33));
        ImGui::SameLine(0, 0);

        if (ImGui::ToggleButton("CAMERA", ImVec2(158, 33), m_SelectedMenu == UIMenu_Camera, false))
            m_SelectedMenu = UIMenu_Camera;

        ImGui::SameLine(0, 20);
        if (ImGui::ToggleButton("VISUALS", ImVec2(158, 33), m_SelectedMenu == UIMenu_Visuals, false))
            m_SelectedMenu = UIMenu_Visuals;

        ImGui::SameLine(0, 20);
        if (ImGui::ToggleButton("MISC", ImVec2(158, 33), m_SelectedMenu == UIMenu_Misc, false))
            m_SelectedMenu = UIMenu_Misc;

        ImGui::PopFont();

        ImGui::GetWindowDrawList()->AddLine(ImVec2(windowPos.x + 143, windowPos.y + 76), ImVec2(windowPos.x + 301, windowPos.y + 76), 0xFF1C79E5, 2);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(windowPos.x + 321, windowPos.y + 76), ImVec2(windowPos.x + 479, windowPos.y + 76), 0xFF1C79E5, 2);
        ImGui::GetWindowDrawList()->AddLine(ImVec2(windowPos.x + 499, windowPos.y + 76), ImVec2(windowPos.x + 657, windowPos.y + 76), 0xFF1C79E5, 2);

        ImGui::Dummy(ImVec2(0, 50));
        ImGui::Dummy(ImVec2(0, 0));
        ImGui::SameLine(10);
        {
            ImGui::BeginChild("contentChild", ImVec2(-10, -10), false);

            if (m_SelectedMenu == UIMenu_Camera)
                g_mainHandle->GetCameraManager()->DrawUI();
            else if (m_SelectedMenu == UIMenu_Visuals)
            {
                ImGui::PushFont(io.Fonts->Fonts[4]);
                ImGui::Dummy(ImVec2(0, 10));
                ImGui::Columns(4, "VisualColumns", false);
                ImGui::NextColumn();
                ImGui::SetColumnOffset(-1, 12);

                ImGui::PushItemWidth(200);

                ImGui::Text("Sun Direction: Hold L to Rotate with Mouse");
                ImGui::InputFloat3("##SunDirection", g_mainHandle->GetCameraManager()->cry3DEngine->m_vSunDirNormalized, "%.2f", 0);
                boost::chrono::duration<double> dt = boost::chrono::high_resolution_clock::now() - lastUpdate;
                lastUpdate = boost::chrono::high_resolution_clock::now();

                if(ImGui::IsKeyDown(ImGuiKey_L)) {

                    g_mainHandle->GetCameraManager()->UpdateSunDir(GetMousePosStart(), dt.count());
                    
                }

                //There are lots more possible vars in the C3DEngine class for someone else to add

                float* shoulder = &g_mainHandle->GetCameraManager()->cry3DEngine->m_fHDRShoulderScale;
                float* midtones = &g_mainHandle->GetCameraManager()->cry3DEngine->m_fHDRMidtonesScale;
                float* toe = &g_mainHandle->GetCameraManager()->cry3DEngine->m_fHDRToeScale;
                float* whitePoint = &g_mainHandle->GetCameraManager()->cry3DEngine->m_fHDRWhitePoint;
                float* contrast = &g_mainHandle->GetCameraManager()->cry3DEngine->m_fHDRContrast;
                float* saturation = &g_mainHandle->GetCameraManager()->cry3DEngine->m_fHDRSaturation;
                float* fogDensity = &g_mainHandle->GetCameraManager()->cry3DEngine->m_volFogGlobalDensity;

                ImGui::Text("Shoulder Scale");
                ImGui::InputFloat("##ShoulderScale", shoulder, 0.01, 0.1, "%.2f", 0);
                ImGui::Text("Midtones Scale");
                ImGui::InputFloat("##MidtonesScale", midtones, 0.01, 0.1, "%.2f", 0);
                ImGui::Text("Toe Scale");
                ImGui::InputFloat("##ToeScale", toe, 0.01, 0.1, "%.2f", 0);
                ImGui::Text("White Point");
                ImGui::InputFloat("##WhitePoint", whitePoint, 0.01, 0.1, "%.2f", 0);
                ImGui::Text("Color Balance");
                ImGui::InputFloat3("##ColorBalance", g_mainHandle->GetCameraManager()->cry3DEngine->m_vColorBalance, "%.2f", 0);

                ImGui::NextColumn();
                ImGui::SetColumnOffset(-1, 290);
                ImGui::PushItemWidth(200);

                ImGui::Text("Contrast");
                ImGui::InputFloat("##Contrast", contrast, 0.01, 0.1, "%.2f", 0);
                ImGui::Text("Saturation");
                ImGui::InputFloat("##Saturation", saturation, 0.01, 0.1, "%.2f", 0);
                ImGui::Text("Fog Density");
                ImGui::InputFloat("##FogDensity", fogDensity, 0.0001, 0.001, "%.4f", 0);
                ImGui::Text("Fog Color");
                ImGui::InputFloat3("##FogColor", g_mainHandle->GetCameraManager()->cry3DEngine->m_vFogColor, "%.2f", 0);


                ImGui::NextColumn();
                ImGui::SetColumnOffset(-1, 552);
                ImGui::PushItemWidth(200);

                ImGui::PopStyleVar();
                ImGui::PopFont();
            }
            else if (m_SelectedMenu == UIMenu_Misc)
            {
                ImGui::Columns(3, "miscColumns", false);
                ImGui::NextColumn();
                ImGui::SetColumnOffset(-1, 5);

                ImGui::Dummy(ImVec2(10, 10));
                ImGui::PushFont(io.Fonts->Fonts[4]);
                ImGui::PushItemWidth(130);

                ImGui::DrawWithBorders([=]
                    {
                        if (ImGui::Button("Config", ImVec2(158, 33)))
                            g_mainHandle->GetInputSystem()->ShowUI();
                    });

                ImGui::NextColumn();
                ImGui::SetColumnOffset(-1, 388.5f);

                ImGui::PushFont(io.Fonts->Fonts[2]);
                ImGui::Dummy(ImVec2(0, 5));
                ImGui::Text("CREDITS"); ImGui::PopFont();

                ImGui::TextWrapped(g_creditsText);

                ImGui::PopFont();
            }
            ImGui::EndChild();
        }

    } ImGui::End(); 

    g_mainHandle->GetInputSystem()->DrawUI();

    ImGui::Render();
    ImGui_ImplDX11_RenderDrawData(ImGui::GetDrawData());

    m_HasKeyboardFocus = ImGui::GetIO().WantCaptureKeyboard;
    m_HasMouseFocus = ImGui::GetIO().WantCaptureMouse;

    
}

void UI::OnResize()
{
    // Hatti: Backbuffer needs to be resized when the window size/resolution changes.
    // However it's bound to the UI's RenderTarget (CreateRenderTarget()), so
    // it needs to be released before the backbuffer can be resized.
    // Failing to handle this will usually result in a crash.

    m_IsResizing = true;
    m_FramesToSkip = 100;
    if (m_pRTV)
        m_pRTV.Reset();
}

void UI::Update(double dt)
{
    // Hatti: For fancy background fading effects
}

void UI::Toggle()
{
    m_Enabled = !m_Enabled;
    if (m_Enabled) {

        GetMousePosStart() = ImGui::GetIO().MousePos;

    }
    
}

bool UI::CreateRenderTarget()
{
    // Hatti: Creates a render target to backbuffer resource
    // Should guarantee that stuff actually gets drawn

    ComPtr<ID3D11Texture2D> pBackBuffer = nullptr;
    HRESULT hr = g_dxgiSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (void**)pBackBuffer.ReleaseAndGetAddressOf());
    if (FAILED(hr) || !pBackBuffer)
    {
        util::log::Error("Failed to retrieve backbuffer from SwapChain, HRESULT 0x%X", hr);
        return false;
    }

    hr = g_d3d11Device->CreateRenderTargetView(pBackBuffer.Get(), NULL, m_pRTV.ReleaseAndGetAddressOf());
    if (FAILED(hr))
    {
        util::log::Error("CreateRenderTargetView failed, HRESULT 0x%X", hr);
        return false;
    }

    return true;
}

ImageRsc UI::CreateImageFromResource(int resourceID)
{
    ImageRsc newImg;

    DWORD szData;
    void* pData;
    if (!util::GetResource(resourceID, pData, szData))
    {
        util::log::Error("CreateImageFromResource failed");
        return newImg;
    }

    HRESULT hr = DirectX::CreateWICTextureFromMemory(g_d3d11Device, (const uint8_t*)pData, szData,
        (ID3D11Resource**)newImg.pTexture.ReleaseAndGetAddressOf(), newImg.pSRV.ReleaseAndGetAddressOf());
    if (FAILED(hr))
        util::log::Error("CreateWICTextureFromMemory failed");

    return newImg;
}