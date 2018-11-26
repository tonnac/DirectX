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
	m_dxrt.set(m_pRenderTargetView, m_pDepthStencilView);
	m_dxrt.Create(m_pd3dDevice, 1024, 1024);

	m_Frustum.Init(m_pd3dDevice);

	D3DXMatrixIdentity(&m_matWorld[0]);
	for (size_t i = 1; i < m_matWorld.size(); ++i)
	{
		D3DXMatrixTranslation(&m_matWorld[i], (float)25 - rand() % 70, 0, (float)25 - rand() % 70);
	}

	m_Plane.Create(m_pd3dDevice, L"shape.hlsl");

	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/misc/dice_unwrap.dds");
	m_pModelCamera = new ModelView;
	m_pMainCamera = m_pModelCamera;

	m_pMainCamera->SetViewMatrix({0, 0, -10 });
	m_pMainCamera->SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);
	m_Frustum.SetMatrix(&m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

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
	m_vp[2].Width = g_rtClient.right * 0.35f;
	m_vp[2].TopLeftY = g_rtClient.bottom * 0.5f;
	m_vp[2].Height = g_rtClient.bottom - m_vp[2].TopLeftY;

	return true;
}
bool Sample::Frame()
{
	m_boxObj.Frame();
	m_Frustum.CreateFrustum();
	return true;
}
bool Sample::Render()
{
	D3DXVECTOR4 vColor = DirectX::Colors::Khaki;

	D3DXMATRIX pp;
	D3DXMatrixLookAtLH(&pp, &D3DXVECTOR3(0, 10.0f, 0.9f), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1.0f, 0));

	D3DXQUATERNION quat = { 0, sinf(g_fGameTimer * 0.5f), 0, cosf(g_fGameTimer * 0.5f) };
	D3DXMATRIX quatrot;
	D3DXMatrixRotationQuaternion(&quatrot, &quat);

	m_dxrt.Begin(m_pImmediateContext, vColor);

	m_boxObj.SetMatrix(&quatrot, &pp, &m_pMainCamera->m_matProj);
	m_boxObj.Render(m_pImmediateContext);

	m_dxrt.End(m_pImmediateContext);

	m_pImmediateContext->RSSetViewports(1, &m_vp[0]);
	D3DXMATRIX dx;

	D3DXMatrixIdentity(&dx);
	m_Plane.SetMatrix(&dx, &dx, &dx);
	m_Plane.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_dxrt.m_pShaderResourceView.GetAddressOf());
	m_Plane.PostRender(m_pImmediateContext);


	D3DXQUATERNION q;
	D3DXQuaternionRotationAxis(&q, &D3DXVECTOR3(0, 1, 0), g_fGameTimer);

	D3DXMATRIX quatrot1;
	D3DXMatrixRotationQuaternion(&quatrot1, &q);

	m_boxObj.SetMatrix(&(dx * quatrot1), &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
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

void Sample::ViewportRender(Shape * pShape)
{
	static D3DXMATRIX matTop;
	static D3DXVECTOR3 vAtTop = { 0.0f, 120.0f, -0.1f };
	static D3DXVECTOR3 vTarget = { 0.0f, 0.0f, 0.0f };
	//D3DXVECTOR3 vAtTop = pShape->m_vPosition;
	//vAtTop.y += 150.0f;
	//vAtTop.z -= 0.1f;

	D3DXMatrixLookAtLH(&matTop,		&vAtTop,	&vTarget, &D3DXVECTOR3(0, 1.0f, 0));

	m_pImmediateContext->RSSetViewports(1, &m_vp[2]);
	for (size_t i = 0; i < m_matWorld.size(); ++i)
	{
		pShape->SetMatrix(&m_matWorld[i], &matTop, &m_pMainCamera->m_matProj);
		pShape->Render(m_pImmediateContext);
	}
	m_Frustum.m_box.SetMatrix(nullptr, &matTop, &m_pMainCamera->m_matProj);
	m_Frustum.Render(m_pImmediateContext);

	D2D1_RECT_F rt;

	rt.top = m_vp[2].TopLeftY;
	rt.left = m_vp[2].TopLeftX;
	rt.right = m_vp[2].Width;
	rt.bottom = m_vp[2].TopLeftY;
	S_DirectWrite.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
	S_DirectWrite.DrawText(rt, L"Top View", { 0.0f,0.0f,0.0f,1.0f });

	rt.top = (float)g_rtClient.bottom - 30;
	rt.bottom = (float)g_rtClient.bottom + 50;

	std::tstring CountText = L"In Frustum : " + std::to_wstring(inFrustumCnt);
	S_DirectWrite.DrawText(rt, CountText.c_str(), { 0.0f,0.0f,0.0f,1.0f });
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
