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
	m_Line.Create(m_pd3dDevice, L"shape.hlsl");

	m_Map.Init();
	MapDesc desc = { 5,5,1.0f,1.0f, L"../../data/map/029_512.jpg",
		L"map.hlsl" };

	m_Map.CreateHeightMap(m_pd3dDevice, m_pImmediateContext, L"../../data/map/heightMap513.bmp");
	desc.iNumRows = m_Map.m_iNumRows;
	desc.iNumCols = m_Map.m_iNumCols;
	desc.fScaleHeight = 1.0f;
	m_Map.Load(m_pd3dDevice, desc);

	return true;
}
bool Sample::Frame()
{
	m_Map.m_cbData.fTime = g_fGameTimer;
	return true;
}
bool Sample::Render()
{
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
