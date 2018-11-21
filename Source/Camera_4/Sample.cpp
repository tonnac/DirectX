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
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/misc/dice_unwrap.png");
	m_pModelCamera = new ModelView;
	m_pMainCamera = m_pModelCamera;

	m_pMainCamera->SetViewMatrix({0, 0, -10 });
	m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);

	for (int i = 0; i < 3; ++i)
	{
		m_vp[i].Height = g_rtClient.bottom / 3.0f;
		m_vp[i].Width = g_rtClient.right * 0.25f;
		m_vp[i].MinDepth = 0.0f;
		m_vp[i].MaxDepth = 1.0f;
		m_vp[i].TopLeftX = 0;
	}

	m_vp[0].TopLeftY = 0;
	m_vp[1].TopLeftY = g_rtClient.bottom / 3.0f;
	m_vp[2].TopLeftY = g_rtClient.bottom / 3.0f * 2.0f;

	m_ViewPort.TopLeftX = g_rtClient.right * 0.25f;
	m_ViewPort.TopLeftY = 0;
	m_ViewPort.Width = g_rtClient.right - m_ViewPort.TopLeftX;
	m_ViewPort.Height = g_rtClient.bottom - m_ViewPort.TopLeftY;
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;

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

	m_boxObj.SetMatrix(&m_pMainCamera->m_matModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_boxObj.Render(m_pImmediateContext);

	ViewportRender(&m_boxObj);
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

void Sample::ViewportRender(Shape * pShape)
{
	static D3DXMATRIX matTop;
	static D3DXMATRIX matFront;
	static D3DXMATRIX matSide;

	D3DXVECTOR3 vAtTop = pShape->m_vPosition;
	vAtTop.y += 10.0f;
	vAtTop.z -= 0.1f;
	D3DXVECTOR3 vAtSide = pShape->m_vPosition;
	vAtSide.x += 10.0f;
	D3DXVECTOR3 vAtFront = pShape->m_vPosition;
	vAtFront.z -= 10.0f;

	D3DXMatrixLookAtLH(&matTop,		&vAtTop,	&pShape->m_vPosition, &D3DXVECTOR3(0, 1.0f, 0));
	D3DXMatrixLookAtLH(&matFront,	&vAtFront,	&pShape->m_vPosition, &D3DXVECTOR3(0, 1.0f, 0));
	D3DXMatrixLookAtLH(&matSide,	&vAtSide,	&pShape->m_vPosition, &D3DXVECTOR3(0, 1.0f, 0));

	m_pImmediateContext->RSSetViewports(1, &m_vp[0]);
	pShape->SetMatrix(nullptr, &matTop, &m_pMainCamera->m_matProj);
	m_Dir.SetMatrix(nullptr, &matTop, &m_pMainCamera->m_matProj);
	pShape->Render(m_pImmediateContext);
	m_Dir.Render(m_pImmediateContext);

	m_pImmediateContext->RSSetViewports(1, &m_vp[1]);
	pShape->SetMatrix(nullptr, &matFront, &m_pMainCamera->m_matProj);
	m_Dir.SetMatrix(nullptr, &matFront, &m_pMainCamera->m_matProj);
	pShape->Render(m_pImmediateContext);
	m_Dir.Render(m_pImmediateContext);

	m_pImmediateContext->RSSetViewports(1, &m_vp[2]);
	pShape->SetMatrix(nullptr, &matSide, &m_pMainCamera->m_matProj);
	m_Dir.SetMatrix(nullptr, &matSide, &m_pMainCamera->m_matProj);
	pShape->Render(m_pImmediateContext);
	m_Dir.Render(m_pImmediateContext);

	D2D1_RECT_F rt;

	rt.top = m_vp[0].TopLeftY;
	rt.left = m_vp[0].TopLeftX;
	rt.right = m_vp[0].Width;
	rt.bottom = m_vp[0].TopLeftY;
	S_DirectWrite.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	S_DirectWrite.DrawText(rt, L"Top View", { 0.0f,0.0f,0.0f,1.0f });

	rt.top = m_vp[1].TopLeftY;
	rt.left = m_vp[1].TopLeftX;
	rt.right = m_vp[1].Width;
	rt.bottom = m_vp[1].TopLeftY;
	S_DirectWrite.DrawText(rt, L"Front View", { 0.0f,0.0f,0.0f,1.0f });

	rt.top = m_vp[2].TopLeftY;
	rt.left = m_vp[2].TopLeftX;
	rt.right = m_vp[2].Width;
	rt.bottom = m_vp[2].TopLeftY;
	S_DirectWrite.DrawText(rt, L"Side View", { 0.0f,0.0f,0.0f,1.0f });
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
