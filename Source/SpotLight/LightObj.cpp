#include "LightObj.h"

void LightObj::SetValue(int iType, D3DXVECTOR3 vPos, D3DXVECTOR3 vDir, float fRange, float fTheta, float fFalloff, D3DXVECTOR4 * vColor, D3DXVECTOR3 * vScale, D3DXVECTOR3 * vRotation)
{
	m_iType = iType;
	m_vInitPos = vPos;
	D3DXVec3Normalize(&m_vInitDir, &vDir);
	DX::CreateConstantBuffer(m_pDevice, 1, sizeof(LIGHT_CONSTANT_BUFFER), m_pCBLight.GetAddressOf());
}

bool LightObj::Init()
{
	return true;
}

void LightObj::Update(D3DXVECTOR3 vEyePos, D3DXVECTOR3 vEyeDir, float intensity, float radius)
{
	m_cbData.g_vEyePos = vEyePos;
	m_cbData.g_vEyeDir = -vEyeDir;
	m_cbData.g_fLightIntensity = intensity;
	m_cbData.g_fLightRadius = radius;
}

bool LightObj::Frame()
{
	D3DXVECTOR3 vLightDir;
	float Phi = (float)D3DX_PI * 0.25f;
	static float Theta = (float)D3DX_PI * 0.25f;
//	Theta += g_fSecPerFrame * (float)D3DX_PI * 0.55f;

	float x = sinf(Phi) * cosf(Theta);
	float y = cosf(Phi);
	float z = sinf(Phi) * sinf(Theta);

//	D3DXVec3Normalize(&m_vInitDir, &-D3DXVECTOR3(x, y, z));
	m_cbData.g_vLightDir = {-x, -y, -z};
	if (m_iType == 2)
	{
		m_cbData.g_vLightDir = -vLightDir;
	//	D3DXVec3Normalize(&m_vInitDir, &-D3DXVECTOR3(x, y, z));
	}
	m_cbData.g_vLightColor = { 0.8f, 0.8f, 0.8f, 1.0f };

	x = 10.0f * sinf(Phi) * cosf(Theta);
	y = 10.0f * cosf(Phi);
	z = 10.0f * sinf(Phi) * sinf(Theta);

	m_cbData.g_vLightPos = m_vCurrentLightPos;
	m_cbData.FalloffStart = 5.0f;
	m_cbData.FalloffEnd = 25.0f;
	return true;
}

bool LightObj::Render(ID3D11DeviceContext * pContext)
{
	pContext->UpdateSubresource(m_pCBLight.Get(), 0, nullptr, &m_cbData, 0, 0);
	pContext->PSSetConstantBuffers(1, 1, m_pCBLight.GetAddressOf());
	return SphereShape::Render(pContext);
}

bool LightObj::Release()
{
	return true;
}
