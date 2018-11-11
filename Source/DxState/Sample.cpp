#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	DxState::SetState(m_pd3dDevice);

	D3DXMatrixIdentity(&m_matWorld[0]);
	D3DXMatrixIdentity(&m_matWorld[1]);
	D3DXMatrixLookAtLH(&m_matView, 
		&D3DXVECTOR3(+0.0f, +0.0f, -10.0f), 
		&D3DXVECTOR3(+0.0f, +0.0f, +0.0f), 
		&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

	for (int i = 0; i < 2; ++i)
	{
		m_vs[i].Width = g_rtClient.right * 0.5f;
		m_vs[i].Height = g_rtClient.bottom * 1.0f;
		m_vs[i].MinDepth = 0.0f;
		m_vs[i].MaxDepth = 1.0f;
	}

	m_vs[0].TopLeftX = 0.0f;
	m_vs[0].TopLeftY = 0.0f;
	m_vs[1].TopLeftX = g_rtClient.right * 0.5f;
	m_vs[1].TopLeftY = 0.0f;

	m_Obj.Init();
	m_Obj.Set(m_pd3dDevice);
	return true;
}
bool Sample::Frame()
{
	m_Obj.Frame();
	return true;
}
bool Sample::Render()
{
	static float dll = 0.0f;
	dll += (float)(0.33f * D3DX_PI) * g_fSecPerFrame;

	m_pImmediateContext->RSSetViewports(1, &m_vs[0]);
	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)E_DSS::Default].Get(), 0);

//	D3DXMatrixRotationY(&m_matWorld[0], dll);
	D3DXMatrixRotationAxis(&m_matWorld[0], &D3DXVECTOR3(1.5f, 2.5f, 0.4f), dll * 3);

	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld[0]);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);

	m_Obj.Render(m_pImmediateContext);

	D3DXMATRIX scale;
	D3DXMATRIX rot;
	D3DXMATRIX tras;
	D3DXMATRIX orbit;
	D3DXMatrixScaling(&scale, 0.6f, 0.6f, 0.6f);
	D3DXMatrixRotationZ(&rot, dll);
	D3DXMatrixTranslation(&tras, 3.0f, 0.0f, 3.0f);
	m_matWorld[1] = scale * rot * tras;
	D3DXMatrixRotationY(&orbit, dll);
	m_matWorld[1] = m_matWorld[1] * orbit;
	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld[1]);

	m_Obj.Render(m_pImmediateContext);

	m_pImmediateContext->RSSetViewports(1, &m_vs[1]);
	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)E_DSS::Disable].Get(), 0);

	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld[0]);
	m_Obj.Render(m_pImmediateContext);

	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld[1]);
	m_Obj.Render(m_pImmediateContext);
	return true;
}
bool Sample::Release()
{
	m_Obj.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
