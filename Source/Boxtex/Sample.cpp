#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <DirectXColors.h>

Sample::Sample() : m_boxObj(true)
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/misc/dice_unwrap.dds");

	m_pMainCamera->SetViewMatrix({0, 0, -10 });
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
	D3DXMATRIX dx;
	D3DXMatrixIdentity(&dx);

	m_boxObj.SetMatrix(&dx, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_boxObj.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	m_boxObj.Release();
	return true;
}

LRESULT	Sample::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	return Window::WndProc(hwnd, msg, wparam, lparam);
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
