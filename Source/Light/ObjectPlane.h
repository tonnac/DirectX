#pragma once
#include "Core.h"
#include "DxObj.h"


struct Light
{
	D3DXVECTOR3 Strength = { 0.5f, 0.5f, 0.5f };
	float FalloffStart = 1.0f;
	D3DXVECTOR3 Direction = { 0.0f, -1.0f, 0.0f };
	float FalloffEnd = 10.0f;
	D3DXVECTOR3 Position = { 0.0f, 0.0f, 0.0f };
	float SpotPower = 64.0f;
};

constexpr std::uint16_t MaxLights = 3;

struct MaterialConstatns
{
	D3DXVECTOR4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;

	D3DMATRIX MatTransform;
};

struct Material
{
	std::string Name;

	D3DXVECTOR4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;

	D3DMATRIX MatTransform;
};

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR3 n;
};

struct CB_DATA
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	float fTime;
	float fActiveVP;
	float z, w;
	D3DXVECTOR3 EyePosW;
	float pad1;
	D3DXVECTOR4 AmbientLight = { 0.25f, 0.25f, 0.25f, 0.25f };
	D3DXVECTOR4 DiffuseAlbedo = { 1.0f, 1.0f, 1.0f, 1.0f };
	D3DXVECTOR3 FresnelR0 = { 0.01f, 0.01f, 0.01f };
	float Roughness = 0.25f;

	Light Lights[MaxLights];
};

class ObjectPlane
{
public:
	DX::DxObj m_DxObject;
	CB_DATA m_cbData;
	std::array<PC_VERTEX, 8> m_VertexList;
	std::array<DWORD, 36> m_indicies;
	UINT m_iIndex;
	UINT m_iVertex;
public:
	bool Init();
	bool Set(ID3D11Device* pDevice);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	ObjectPlane();
	virtual ~ObjectPlane();
};