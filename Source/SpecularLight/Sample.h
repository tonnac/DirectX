#pragma once
#include "Core.h"
#include "Map.h"
#include "Frustum.h"

#define MaxLights 16

struct LIGHT_CONSTANT_BUFFER
{
	D3DXMATRIX g_matInvWorld;
	D3DXVECTOR3 g_vLightDir;
	float		g_fLightIntensity;
	D3DXVECTOR3 g_vLightPos;
	float		g_fLightRadius;
	D3DXVECTOR4 g_vLightStrength;
	D3DXVECTOR3 g_vEyeDir;
	float		FalloffEnd;
	D3DXVECTOR3 g_vEyePos;
	float		FalloffStart;
};

struct Lights
{
	LIGHT_CONSTANT_BUFFER lights[MaxLights];
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
	void		UpdateLights();
private:
	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBLight;
	Lights m_cbData;

	Map			m_Map;
};
