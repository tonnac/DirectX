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
	m_Frustum.Init(m_pd3dDevice);
	m_dxrt1.Create(m_pd3dDevice, 1024, 1024);

	D3DXMatrixIdentity(&m_matWorld[0]);
	for (size_t i = 1; i < m_matWorld.size(); ++i)
	{
		D3DXMatrixTranslation(&m_matWorld[i], (float)25 - rand() % 70, 0, (float)25 - rand() % 70);
	}

	m_Plane.Create(m_pd3dDevice, L"shape.hlsl");
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/misc/dice_unwrap.dds");

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

	D3DXMATRIX look;
	D3DXMatrixLookAtLH(&look, &D3DXVECTOR3(0.0f, 100.0f, 0.1f), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	m_Frustum.SetMatrix(&look, &m_pMainCamera->m_matProj);

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
	D3DXVECTOR4 vColor = D3DXVECTOR4(DirectX::Colors::Khaki);

	D3DXMATRIX pp;
	D3DXMatrixLookAtLH(&pp, &D3DXVECTOR3(0, 100.0f, 0.9f), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1.0f, 0));

	D3DXQUATERNION quat = { 0, sinf(g_fGameTimer * 0.5f), 0, cosf(g_fGameTimer * 0.5f) };
	D3DXMATRIX quatrot;
	D3DXMatrixRotationQuaternion(&quatrot, &quat);

	m_dxrt1.Begin(m_pImmediateContext, vColor);

	//m_boxObj.SetMatrix(&quatrot, &pp, &m_pMainCamera->m_matProj);
	//m_boxObj.Render(m_pImmediateContext);
	m_Frustum.SetMatrix(&pp, &m_pMainCamera->m_matProj);
	m_Frustum.Render(m_pImmediateContext);

	m_dxrt1.End(m_pImmediateContext, &m_dxRt);


	D3DXMATRIX dx;
	D3DXMatrixIdentity(&dx);
	m_Plane.SetMatrix(&dx, &dx, &dx);
	m_pImmediateContext->RSSetViewports(1, &m_vp[0]);
	m_Plane.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_dxrt1.m_pShaderResourceView.GetAddressOf());
	m_Plane.PostRender(m_pImmediateContext);

	m_boxObj.SetMatrix(&m_Camera["modelview"]->m_matModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_pImmediateContext->RSSetViewports(1, &m_dxRt.m_Viewport);
	m_boxObj.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	m_boxObj.Release();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
