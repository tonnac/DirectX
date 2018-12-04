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
	m_SpotLightObj.Create(m_pd3dDevice, L"maplight.hlsl", L"../../data/effect/Particle6.dds");

	D3DXVECTOR3 vLighsPos = { 100, 100, 100 };
	D3DXVECTOR3 vLightDir;
	D3DXVec3Normalize(&vLightDir, &vLighsPos);
	m_SpotLightObj.SetValue(1, vLighsPos, vLightDir);

	m_Map.Init();
	MapDesc desc = { 5,5,1.0f,1.0f, L"../../data/map/castle.jpg",
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
	m_SpotLightObj.Update(m_pMainCamera->m_vPos, m_pMainCamera->m_vLook);
	m_SpotLightObj.Frame();
	return true;
}
bool Sample::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);
	static float dll = 0.0f;
	dll += g_fSecPerFrame * 0.25 * D3DX_PI;

	D3DXMATRIX t;
	D3DXMATRIX r;
	D3DXMATRIX s;
	D3DXMatrixScaling(&s, 1.7f, 1.7f, 1.7f);
	D3DXMatrixRotationY(&r, dll);
	D3DXMatrixTranslation(&t, 20, 0, 20);
	m_SpotLightObj.m_matWorld = s * t * r;

	m_SpotLightObj.m_matWorld._42 = m_Map.GetHeight(m_SpotLightObj.m_matWorld._41, m_SpotLightObj.m_matWorld._43) + m_SpotLightObj.m_fOffsetHeight;

	m_SpotLightObj.m_vCurrentLightPos = { m_SpotLightObj.m_matWorld._41, m_SpotLightObj.m_matWorld._42, m_SpotLightObj.m_matWorld._43 };

	m_SpotLightObj.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_SpotLightObj.Render(m_pImmediateContext);

	m_Map.SetMatrix(&mat, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Map.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	m_SpotLightObj.Release();
	m_Map.Release();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
