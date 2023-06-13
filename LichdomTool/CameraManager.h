#pragma once
#include "TrackPlayer.h"
#include "INIReader.h"
#include "CameraStructs.h"
#include "CryLich.h"
#include "../imgui/imgui.h"
#include "../imgui/imgui_impl_dx11.h"
#include "../imgui/imgui_impl_win32.h"
#include <vector>

class CameraManager
{
public:
	CameraManager();
	~CameraManager();

	void HotkeyUpdate();
	void Update(double dt);
	void DrawUI();

	bool IsCameraEnabled() { return m_CameraEnabled; }
	bool IsGamepadDisabled() { return m_CameraEnabled && m_GamepadDisabled; };
	bool IsKbmDisabled() { return m_CameraEnabled && m_KbmDisabled; };
	bool IsTimeFrozen() { return m_TimeFreezeEnabled; };

	bool IsHUDEnabled() { return m_HUDEnabled; }

	void UpdateSunDir(ImVec2 mousePos, double dt);

	void UpdateResolution(int x, int y);

	void ChangeResolution();

	typedef struct Screen {
		int x;
		int y;
	} m_screen;

	m_screen FindScreenSize();

	std::vector<int> GetResListWidth() { return resListWidth; };
	std::vector<int> GetResListHeight() { return resListHeight; };

	bool OverrideRatio() { return m_OverrideAspectRatio; }
	float GetAspectRatio() { return m_AspectRatio; }

	DirectX::XMFLOAT3X4& GetCameraTrans() { return m_Camera.Transform; }
	DirectX::XMMATRIX& GetMatrix() { return m_Camera.Matrix; }
	DirectX::XMFLOAT3& GetPosition() { return m_Camera.Position; };
	DirectX::XMFLOAT4& GetRotation() { return m_Camera.Rotation; };

	float GetCameraFov() { return DirectX::XMConvertToRadians(m_Camera.FieldOfView); }
	void SetResetMatrix(DirectX::XMFLOAT3X4 const& m) { m_ResetMatrix = m; }

	void ReadConfig(INIReader* pReader);
	const std::string GetConfig();

	bool m_CameraEnabled;

	Screen setScreen;

	CryLich::CCamera* cryCamera;
	CryLich::C3DEngine* cry3DEngine;
	CryLich::CView* cryView;
	CryLich::CFlashUI* cryUI;
	CryLich::IConsole* cryConsole;
	CryLich::CSystem* crySystem;
	CryLich::CD3D9Renderer* cryD3D9Renderer;
	CryLich::CCryAction* cryAction;
	CryLich::CHardwareMouse* cryMouse;

	DirectX::XMFLOAT3X4 *m_gameMatrixLocation;

	

private:
	void UpdateCamera(double dt);
	void UpdateInput(double dt);
	

	void ToggleCamera();
	void ResetCamera();

	void ToggleHUD();

private:
	
	bool m_FirstEnable;
	bool m_AutoReset;

	bool m_GamepadDisabled;
	bool m_KbmDisabled;

	bool m_HUDEnabled;

	bool m_TimeFreezeEnabled;
	bool m_OverrideAspectRatio;
	float m_AspectRatio;

	float m_pitch;
	float m_yaw;
	float m_roll;

	std::vector<int> resListWidth;
	std::vector<int> resListHeight;

	bool m_UiRequestReset;
	DirectX::XMFLOAT3X4 m_ResetMatrix;
	
	Camera m_Camera;

	
	DepthOfField m_DepthOfField;
	TrackPlayer m_TrackPlayer;

public:
	CameraManager(CameraManager const&) = delete;
	void operator=(CameraManager const&) = delete;
};

