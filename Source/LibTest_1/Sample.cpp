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
	return true;
}
bool Sample::Frame()
{
	m_boxObj.Frame();

	D3DXMATRIX dx;
	D3DXMatrixIdentity(&dx);
	m_boxObj.SetMatrix(&m_Camera["modelview"]->m_matModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	return true;
}
bool Sample::Render()
{
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
	if(m_Camera["modelview"] != nullptr)
		m_Camera["modelview"]->MsgProc(hwnd, msg, wparam, lparam);
	return Window::WndProc(hwnd, msg, wparam, lparam);
}

void Sample::DeleteResources()
{
	return;
}
HRESULT Sample::CreateResources(const UINT& Width, const UINT& Height)
{
	return S_OK;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
