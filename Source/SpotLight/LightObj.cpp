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
	D3DXVECTOR3 vLightPos, vLightDir;
	static float dll = 0.0f;
	dll += g_fSecPerFrame * 0.125f * D3DX_PI;

	D3DXMATRIX t;
	D3DXMATRIX r;
	D3DXMatrixRotationY(&r, dll);
	D3DXVec3TransformCoord(&vLightPos, &m_vInitPos, &r);

	if (m_iType == 2)
	{
		m_cbData.g_vLightDir = -vLightDir;
	//	D3DXVec3Normalize(&m_vInitDir, &-D3DXVECTOR3(x, y, z));
	}
	m_cbData.g_vLightColor = { 0.8f, 0.8f, 0.8f, 1.0f };

	m_vCurrentLightPos.x = vLightPos.x;
	m_vCurrentLightPos.z = vLightPos.z;

	m_cbData.g_vLightPos = m_vCurrentLightPos;
	m_cbData.FalloffStart = 5.0f;
	m_cbData.FalloffEnd = 35.0f;
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
