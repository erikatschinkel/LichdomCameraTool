#pragma once
#include <d3d11.h>
#include <DirectXMath.h>
#include <vector>
#include <wrl.h>
#include <string>


struct CatmullRomNode
{
    DirectX::XMFLOAT3 Position;
    DirectX::XMFLOAT4 Rotation;
    float FieldOfView;
    float TimeStamp;
};

struct Camera
{
    DirectX::XMFLOAT3 Position{ 0,0,0 };
    DirectX::XMFLOAT4 Rotation{ 0,0,0,1 };    float FieldOfView{ 50.f };

    float MovementSpeed{ 3.0f };
    float RotationSpeed{ DirectX::XM_PI / 4 };
    float RollSpeed{ DirectX::XM_PI / 8 };
    float FovSpeed{ 5.0f };

    float dX{ 0 };
    float dY{ 0 };
    float dZ{ 0 };
    float dPitch{ 0 };
    float dYaw{ 0 };
    float dRoll{ 0 };
    float dFov{ 0 };

    DirectX::XMMATRIX Matrix{ DirectX::XMMatrixIdentity() };

    DirectX::XMFLOAT3X4 Transform{ 1,0,0,0,
                                    0,1,0,0,
                                    0,0,1,0};
};

struct CameraTrack
{
    std::string Name;
    std::vector<CatmullRomNode> Nodes;
    Microsoft::WRL::ComPtr<ID3D11Buffer> VertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> IndexBuffer;
    int IndexCount;

    CameraTrack(std::string const& name)
    {
        Name = name;
    }
};

struct DepthOfField
{
    float BokehScale{ 3.f };
    float FStop{ 2.5f };
    float FocalDistance{ 10.f };
    bool Enabled{ false };
};
