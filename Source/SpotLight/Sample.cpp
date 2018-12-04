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
	m_SpotLightObj.Create(m_pd3dDevice, L"maplight.hlsl", L"../../data/misc/checker_with_numbers.bmp");

	D3DXVECTOR3 vLighsPos = { 100, 100, 100 };
	D3DXVECTOR3 vLightDir;
	D3DXVec3Normalize(&vLightDir, &vLighsPos);
	m_SpotLightObj.SetValue(1, vLighsPos, vLightDir);

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
	m_SpotLightObj.Update(m_pMainCamera->m_vPos, m_pMainCamera->m_vLook);
	m_SpotLightObj.Frame();
	return true;
}
bool Sample::Render()
{
	D3DXMATRIX mat;
	D3DXMatrixIdentity(&mat);

	m_SpotLightObj.SetMatrix(&mat, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
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
