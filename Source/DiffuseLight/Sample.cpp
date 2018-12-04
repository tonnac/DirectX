#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <DirectXColors.h>

using namespace DirectX;


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	DX::CreateConstantBuffer(m_pd3dDevice, 1, sizeof(LIGHT_CONSTANT_BUFFER), m_pCBLight.GetAddressOf());

	m_Map.Init();
	MapDesc desc = { 5,5,1.0f,1.0f, L"../../data/map/029_512.jpg",
		L"maplight.hlsl" };

	m_Map.CreateHeightMap(m_pd3dDevice, m_pImmediateContext, L"../../data/map/HEIGHT_CASTLE.bmp");
	desc.iNumRows = m_Map.m_iNumRows;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.fScaleHeight = 0.1f;
	m_Map.Load(m_pd3dDevice, desc);

	return true;
}
bool Sample::Frame()
{
	D3DXVec3Normalize(&m_vLightDir, &m_vLightPos);

	m_cbData.g_vLightPos.x = m_vLightPos.x;
	m_cbData.g_vLightPos.y = m_vLightPos.y;
	m_cbData.g_vLightPos.z = m_vLightPos.z;
	m_cbData.g_vLightPos.w = 50.0f; // radius

	m_cbData.g_vLightDir.x = m_vLightDir.x;
	m_cbData.g_vLightDir.y = m_vLightDir.y;
	m_cbData.g_vLightDir.z = m_vLightDir.z;
	m_cbData.g_vLightDir.w = 10.0f; // intensity(세기, 강도)

	D3DXMatrixInverse(&m_cbData.g_matInvWorld, nullptr, &m_Map.m_matWorld);

	m_cbData.g_vEyePos.x = m_pMainCamera->m_vPos.x;
	m_cbData.g_vEyePos.y = m_pMainCamera->m_vPos.y;
	m_cbData.g_vEyePos.z = m_pMainCamera->m_vPos.z;

	D3DXVECTOR4 vPos = { m_pMainCamera->m_vPos.x, m_pMainCamera->m_vPos.y, m_pMainCamera->m_vPos.z, 0.0f };
	D3DXVec4Normalize(&m_cbData.g_vEyeDir, &vPos);

	m_Map.m_cbData.fTime = g_fGameTimer;
	return true;
}
bool Sample::Render()
{
	m_pImmediateContext->UpdateSubresource(m_pCBLight.Get(), 0, nullptr, &m_cbData, 0, 0);

	m_pImmediateContext->VSSetConstantBuffers(1, 1, m_pCBLight.GetAddressOf());

	m_Map.SetMatrix(&m_Camera["modelview"]->m_matModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Map.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
