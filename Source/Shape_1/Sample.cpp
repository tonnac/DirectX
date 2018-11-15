#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <DirectXColors.h>


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_DefaultCamera.SetViewMatrix({ 5,5,-10.0f });
	m_DefaultCamera.SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);

	m_pMainCamera = &m_DefaultCamera;

	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_plane.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
//	m_line.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_Dir.Create(m_pd3dDevice, L"shape.hlsl");
	return true;
}
bool Sample::Frame()
{
	m_boxObj.Frame();
	m_plane.Frame();
	return true;
}
bool Sample::Render()
{
	static float dll = 0.0f;
	dll += (float)(0.33f * D3DX_PI) * g_fSecPerFrame;
	
	D3DXMATRIX rot;
	D3DXMatrixRotationY(&rot, dll);

//	m_pMainCamera->SetViewMatrix(D3DXVECTOR3(1, 2, -10));

	D3DXMatrixIdentity(&m_matWorld[0]);

	m_boxObj.m_cbData.fTime = g_fGameTimer;

	m_boxObj.SetMatrix(&m_matWorld[0], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	D3DXMatrixTranslation(&m_matWorld[1], 4, 0, 0.0f);
	m_plane.SetMatrix(&m_matWorld[1], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_boxObj.Render(m_pImmediateContext);
	m_plane.Render(m_pImmediateContext);

	m_Dir.SetMatrix(&m_matWorld[0], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Dir.Render(m_pImmediateContext);
//	m_line.Draw(m_pImmediateContext, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 1000, 0), D3DXVECTOR4(DirectX::Colors::Blue));
//	m_line.Draw(m_pImmediateContext, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(0, 0, 1000), D3DXVECTOR4(DirectX::Colors::Green));
//	m_line.Draw(m_pImmediateContext, D3DXVECTOR3(0, 0, 0), D3DXVECTOR3(1000, 0, 0), D3DXVECTOR4(DirectX::Colors::Red));
	return true;
}
bool Sample::Release()
{
	m_boxObj.Release();
	m_plane.Release();
	return true;
}

void Sample::DeleteResources()
{
	return;
}
HRESULT Sample::CreateResources(const UINT& Width, const UINT& Height)
{
	m_pMainCamera->UpdateProjMatrix(Width, Height);
	return S_OK;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
