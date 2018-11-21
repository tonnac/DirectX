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
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_pModelCamera = new ModelView;
	m_pMainCamera = m_pModelCamera;

	m_pMainCamera->SetViewMatrix({ 5,10, 1.0f });
	m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);
	return true;
}
bool Sample::Frame()
{
	m_boxObj.Frame();
	return true;
}
bool Sample::Render()
{
	static float dll = 0.0f;
	dll += (float)(0.15f * D3DX_PI) * g_fSecPerFrame;

	D3DXMatrixIdentity(&m_boxObj.m_SRTMat.matWorld);
	ModelView * pView = dynamic_cast<ModelView*>(m_pMainCamera);
	m_boxObj.SetMatrix(&pView->m_matModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_boxObj.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	m_boxObj.Release();
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

LRESULT	Sample::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	m_pModelCamera->MsgProc(hwnd, msg, wparam, lparam);
	return Window::WndProc(hwnd, msg, wparam, lparam);
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
