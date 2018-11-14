#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	D3D11_DEPTH_STENCIL_DESC dsDesc;
	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsDesc.StencilEnable = TRUE;
	dsDesc.StencilReadMask = 0xff;
	dsDesc.StencilWriteMask = 0xff;

	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_INCR;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_pd3dDevice->CreateDepthStencilState(&dsDesc, &m_DSS[0]);

	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;

	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
	dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;
	m_pd3dDevice->CreateDepthStencilState(&dsDesc, &m_DSS[1]);

	dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_LESS_EQUAL;

	dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_GREATER_EQUAL;
	m_pd3dDevice->CreateDepthStencilState(&dsDesc, &m_DSS[2]);

	DxState::InitState(m_pd3dDevice);

	D3DXMatrixIdentity(&m_matWorld[0]);
	D3DXMatrixIdentity(&m_matWorld[1]);
	D3DXMatrixLookAtLH(&m_matView, 
		&D3DXVECTOR3(0.0f, +5.0f, -10.0f), 
		&D3DXVECTOR3(+0.0f, +0.0f, +0.0f), 
		&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);


	m_box.Create(m_pd3dDevice, L"ee", L"ee");
	return true;
}
bool Sample::Frame()
{
	m_box.Frame();
	return true;
}
bool Sample::Render()
{
	static float dll = 0.0f;
	dll += (float)(0.33f * D3DX_PI) * g_fSecPerFrame;
	

	D3DXMATRIX rot;

	D3DXMatrixRotationY(&rot, dll);
	D3DXMatrixTranslation(&m_matWorld[0], 0, 0, -3.0f);
	m_matWorld[0] = m_matWorld[0] * rot;
	D3DXMatrixTranspose(&m_box.m_cbData.matWorld, &m_matWorld[0]);
	m_pImmediateContext->RSSetState(DxState::m_RSS[(int)E_RSS::SolidBack].Get());
	m_pImmediateContext->OMSetBlendState(DxState::m_BSS[(int)E_BSS::No].Get(), 0, 0xffffffff);
	m_pImmediateContext->OMSetDepthStencilState(m_DSS[0], 1);
	m_box.Render(m_pImmediateContext);

	m_pImmediateContext->OMSetDepthStencilState(m_DSS[1], 1);
	m_pImmediateContext->OMSetBlendState(DxState::m_BSS[(int)E_BSS::Default].Get(), 0, 0xffffffff);
	D3DXMATRIX Scale;
	D3DXMatrixScaling(&Scale, 10, 10, 1);
	D3DXMatrixTranslation(&m_matWorld[0], 0, 0, -5.0f);
	m_matWorld[0] = Scale * m_matWorld[0];
	D3DXMatrixTranspose(&m_box.m_cbData.matWorld, &m_matWorld[0]);
	D3DXMatrixTranspose(&m_box.m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_box.m_cbData.matProj, &m_matProj);
	m_box.PreRender(m_pImmediateContext);
//	m_Obj.PostRender(m_pImmediateContext);
	m_pImmediateContext->DrawIndexed(6, 0, 0);
	return true;
}
bool Sample::Release()
{
	m_box.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
