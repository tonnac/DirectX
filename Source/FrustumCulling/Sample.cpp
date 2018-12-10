#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <DirectXColors.h>

using namespace DirectX;


Sample::Sample() : m_boxObj(true)
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_Frustum.Init(m_pd3dDevice);
	m_dxrt1.Create(m_pd3dDevice, 1024, 1024);

	D3DXMatrixIdentity(&m_worldarray[0]);

	D3DXMATRIX S, R, T;
	for (int iBox = 0; iBox < 100; iBox++)
	{
		D3DXMatrixTranslation(&T, 50 - rand() % 100, 50 - rand() % 100, 50 - rand() % 100);
		// g_matWorld Matrix = S * R
		D3DXMatrixScaling(&S, (rand() % 256) / 255.0f * 3.0f, (rand() % 256) / 255.0f * 3.0f, (rand() % 256) / 255.0f * 3.0f);
		D3DXMatrixRotationYawPitchRoll(&R, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f, (rand() % 256) / 255.0f);
		D3DXMATRIX World = S * T * R;
		m_worldarray[iBox] = World;
	}
	m_Plane.Create(m_pd3dDevice, L"shape.hlsl");
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/misc/dice_unwrap.dds");
	m_Frustum.SetMatrix(&m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_vp.TopLeftY = g_rtClient.bottom * 0.6f;
	m_vp.TopLeftX = 0.0f;
	m_vp.Width = g_rtClient.right * 0.35f;
	m_vp.Height = g_rtClient.bottom * 0.4f;
	m_vp.MinDepth = 0.0f;
	m_vp.MaxDepth = 1.0f;
	

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
	static D3DXMATRIX look;
	D3DXMatrixLookAtLH(&look, &D3DXVECTOR3(0.0f, 250.0f, -0.1f), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	m_Frustum.SetMatrix(&m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	D3DXVECTOR4 vColor = D3DXVECTOR4(DirectX::Colors::White);

	D3DXQUATERNION quat = { 0, sinf(g_fGameTimer * 0.5f), 0, cosf(g_fGameTimer * 0.5f) };
	D3DXMATRIX quatrot;
	D3DXMatrixRotationQuaternion(&quatrot, &quat);

	m_dxrt1.Begin(m_pImmediateContext, vColor);

	m_boxObj.SetMatrix(&quatrot, &look, &m_pMainCamera->m_matProj);
	m_boxObj.Render(m_pImmediateContext);
	m_Frustum.m_box.SetMatrix(nullptr, &look, &m_pMainCamera->m_matProj);
	m_Frustum.Render(m_pImmediateContext);

	m_dxrt1.End(m_pImmediateContext, &m_dxRt);


	D3DXMATRIX dx;
	D3DXMatrixIdentity(&dx);
	m_Plane.SetMatrix(&dx, &dx, &dx);
	m_pImmediateContext->RSSetViewports(1, &m_vp);
	m_Plane.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_dxrt1.m_pShaderResourceView.GetAddressOf());
	m_Plane.PostRender(m_pImmediateContext);

	for (size_t i = 0; i < m_worldarray.size(); ++i)
	{
		m_boxObj.SetMatrix(&m_worldarray[i], &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		m_pImmediateContext->RSSetViewports(1, &m_dxRt.m_Viewport);
		m_boxObj.Render(m_pImmediateContext);
	}
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
