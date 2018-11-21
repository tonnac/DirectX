#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	DxState::InitState(m_pd3dDevice);

	D3DXMatrixIdentity(&m_matWorld[0]);
	D3DXMatrixIdentity(&m_matWorld[1]);
	D3DXMatrixLookAtLH(&m_matView, 
		&D3DXVECTOR3(0.0f, +5.0f, -5.0f), 
		&D3DXVECTOR3(+0.0f, +0.0f, +0.0f), 
		&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

	//for (int i = 0; i < 2; ++i)
	//{
	//	m_vs[i].Width = g_rtClient.right * 0.5f;
	//	m_vs[i].Height = g_rtClient.bottom * 1.0f;
	//	m_vs[i].MinDepth = 0.0f;
	//	m_vs[i].MaxDepth = 1.0f;
	//}

	//m_vs[0].TopLeftX = 0.0f;
	//m_vs[0].TopLeftY = 0.0f;
	//m_vs[1].TopLeftX = g_rtClient.right * 0.5f;
	//m_vs[1].TopLeftY = 0.0f;


	m_Obj.Init();
	m_Obj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
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
	

	D3DXMATRIX rot;

	D3DXMatrixRotationY(&rot, (float)D3DX_PI * 0.25f);
	D3DXMatrixTranslation(&m_matWorld[0], 0, 0, 0.0f);
	m_matWorld[0] = rot * m_matWorld[0];
	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld[0]);
	m_pImmediateContext->RSSetState(DxState::m_RSS[(int)E_RSS::SolidBack].Get());
	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)E_DSS::Default].Get(), 0);
	m_Obj.Render(m_pImmediateContext);


	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, 2, 2, 2);
	D3DXMatrixTranslation(&m_matWorld[0], 0, 0, 1.0f);
	m_matWorld[0] = Scale * m_matWorld[0];
	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld[0]);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);
	m_pImmediateContext->RSSetState(DxState::m_RSS[(int)E_RSS::SolidBack].Get());
	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)E_DSS::Default].Get(), 0);
	m_Obj.PreRender(m_pImmediateContext);
//	m_Obj.PostRender(m_pImmediateContext);
	m_pImmediateContext->DrawIndexed(6, 0, 0);
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
