#pragma once

#include <memory>
#include <Windows.h>

#include "CameraStructs.h"
#include "CameraManager.h"
#include "InputSystem.h"
#include "INIReader.h"
#include "UI.h"
#include "Minhook.h"
#include "CryLich.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"

#include <boost/filesystem.hpp>
#include <boost/chrono.hpp>



class Main {
public:
	Main();
	~Main();

	bool Initialize();
	void Run();

	CameraManager* GetCameraManager() { return m_pCameraManager.get(); }
	InputSystem* GetInputSystem() { return m_pInputSystem.get(); }
	UI* GetUI() { return m_pUI.get(); }

	void OnConfigChanged() { m_ConfigChanged = true; }

private:
	void LoadConfig();
	void SaveConfig();

	static LRESULT CALLBACK WndProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

private:
	boost::chrono::high_resolution_clock::time_point m_dtUpdate;

	std::unique_ptr<INIReader> m_pConfig;

	std::unique_ptr<CameraManager> m_pCameraManager;
	std::unique_ptr<InputSystem> m_pInputSystem;
	std::unique_ptr<UI> m_pUI;

	bool m_Initialized;
	bool m_ConfigChanged;
	float m_dtConfigCheck;

public:
	Main(Main const&) = delete;
	void operator=(Main const&) = delete;
};

extern Main* g_mainHandle;

extern bool g_shutdown;
extern bool g_hasFocus;
extern bool g_uiOwnsKeyboard;
extern bool g_uiOwnsMouse;

extern CryLich::SCVars* g_pGameCVars;

extern HINSTANCE g_dllHandle;
extern HINSTANCE g_gameHandle;
extern HWND g_gameHwnd;
extern WNDPROC g_origWndProc;

extern ID3D11Device* g_d3d11Device;
extern ID3D11DeviceContext* g_d3d11Context;
extern IDXGISwapChain* g_dxgiSwapChain;
