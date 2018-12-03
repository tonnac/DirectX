#pragma once
#include "Core.h"
#include "Map.h"
#include "Frustum.h"

struct LIGHT_CONSTANT_BUFFER
{
	D3DXMATRIX g_matInvWorld;
	D3DXVECTOR4 g_vLightDir;
	D3DXVECTOR4 g_vLightPos;
	D3DXVECTOR4 g_vEyeDir;
	D3DXVECTOR4 g_vEyePos;
};

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool	   	Init() override;
	bool	   	Frame() override;
	bool	   	Render() override;
	bool	   	Release() override;
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBLight;
	LIGHT_CONSTANT_BUFFER m_cbData;

	D3DXVECTOR3 m_vLightPos = { 100.0f, 100.0f, 100.0f };
	D3DXVECTOR3 m_vLightDir;

	Map			m_Map;
};
