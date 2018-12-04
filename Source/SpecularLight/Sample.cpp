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
	DX::CreateConstantBuffer(m_pd3dDevice, 1, sizeof(Lights), m_pCBLight.GetAddressOf());

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
	UpdateLights();
	m_Map.m_cbData.fTime = g_fGameTimer;
	return true;
}
bool Sample::Render()
{
	m_pImmediateContext->UpdateSubresource(m_pCBLight.Get(), 0, nullptr, &m_cbData, 0, 0);

	m_pImmediateContext->PSSetConstantBuffers(1, 1, m_pCBLight.GetAddressOf());
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	m_Map.SetMatrix(&mat, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Map.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	return true;
}

void Sample::UpdateLights()
{
	float Phi = (float)D3DX_PI * 0.25f;
	static float Theta = (float)D3DX_PI * 0.25f;
	Theta += g_fSecPerFrame * (float)D3DX_PI * 0.55f;

	float x = sinf(Phi) * cosf(Theta);
	float y = cosf(Phi);
	float z = sinf(Phi) * sinf(Theta);

	D3DXVec3Normalize(&m_cbData.lights[0].g_vLightDir, &-D3DXVECTOR3(x, y, z));
	m_cbData.lights[0].g_vLightStrength = { 0.8f, 0.8f, 0.8f, 1.0f };
	m_cbData.lights[0].g_vEyePos = m_pMainCamera->m_vPos;
	m_cbData.lights[0].g_vEyeDir = -m_pMainCamera->m_vLook;

	x = 100.0f * sinf(Phi) * cosf(Theta);
	y = 100.0f * cosf(Phi);
	z = 100.0f * sinf(Phi) * sinf(Theta);

	m_cbData.lights[1].g_vLightStrength = { 0.8f, 0.8f, 0.8f, 1.0f };
	m_cbData.lights[1].g_vLightPos = { x, y, z };
	m_cbData.lights[1].FalloffStart = 10.0f;
	m_cbData.lights[1].FalloffEnd = 100.0f;
	m_cbData.lights[1].g_vEyePos = m_pMainCamera->m_vPos;
	m_cbData.lights[1].g_vEyeDir = -m_pMainCamera->m_vLook;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
