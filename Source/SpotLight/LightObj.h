#pragma once
#include "Shape.h"

struct LIGHT_CONSTANT_BUFFER
{
	D3DXVECTOR3 g_vLightDir;
	float		g_fLightIntensity;
	D3DXVECTOR3 g_vLightPos;
	float		g_fLightRadius;
	D3DXVECTOR3 g_vEyeDir;
	float		FalloffEnd;
	D3DXVECTOR3 g_vEyePos;
	float		FalloffStart;
	D3DXVECTOR4 g_vSpotInfo;
	D3DXVECTOR4 g_vLightColor;
	// x = theta, y = phi, z= falloff, w = count
};

//struct LightInfo
//{
//	D3DXVECTOR4 m_vDiffuseColor;
//	D3DXVECTOR3 m_vPos;
//	D3DXVECTOR3 m_vDir;
//	float		m_fRange;
//	float		m_fFalloff;
//	float		m_fTheta;
//	float		m_fPhi;
//};

class LightObj : public SphereShape  // sphere
{
public:
	int			m_iType; // 0 : direction, 1 : point, 2 : spot
	D3DXMATRIX	m_matWorld1;
	D3DXMATRIX	m_vInitPosition;
	D3DXVECTOR3 m_vScale;
	D3DXVECTOR3 m_vRotation;
	float		m_fOffsetHeight = 5.0f;
	D3DXVECTOR3 m_vInitPos;
	D3DXVECTOR3 m_vInitDir;
	D3DXVECTOR3 m_vCurrentLightPos;

	Microsoft::WRL::ComPtr<ID3D11Buffer> m_pCBLight;
	LIGHT_CONSTANT_BUFFER m_cbData;
public:
	void SetValue(
		int iType,
		D3DXVECTOR3 vPos,
		D3DXVECTOR3 vDir,
		float fRange = 100.0f, 
		float fTheta = (float)D3DX_PI * 0.25f,
		float fFalloff = 5.0f,
		D3DXVECTOR4* vColor = nullptr, 
		D3DXVECTOR3* vScale = nullptr,
		D3DXVECTOR3* vRotation = nullptr);
	bool Init();
	bool Frame();
	// Frame전에 호출
	void Update(D3DXVECTOR3 vEyePos, D3DXVECTOR3 vEyeDir, float intensity = 10.0f, float radius = 100.0f);
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
};