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
	m_Map.Init();
	MapDesc desc = { 250,250,1.0f,1.0f, L"../../data/map/Dirt_Diff.dds",
		L"shape.hlsl" };
	m_Map.Load(m_pd3dDevice, desc);

	m_dxrt1.Create(m_pd3dDevice, 1024, 1024);

	D3DXMatrixIdentity(&m_matWorld[0]);
	for (size_t i = 1; i < m_matWorld.size(); ++i)
	{
		D3DXMatrixTranslation(&m_matWorld[i], (float)25 - rand() % 70, 0, (float)25 - rand() % 70);
	}

	m_Plane.Create(m_pd3dDevice, L"shape.hlsl");
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/misc/dice_unwrap.dds");
	m_Frustum.Init(m_pd3dDevice);


	m_vp.MaxDepth = 1.0f;
	m_vp.MinDepth = 0.0f;
	m_vp.TopLeftX = 0;
	m_vp.TopLeftY = g_rtClient.bottom * 0.6f;
	m_vp.Width = g_rtClient.right * 0.35f;
	m_vp.Height = g_rtClient.bottom * 0.4f;

	return true;
}
bool Sample::Frame()
{
	m_Frustum.SetMatrix(&m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Frustum.CreateFrustum();
	m_boxObj.Frame();
	return true;
}
bool Sample::Render()
{
	static D3DXMATRIX look;
	D3DXMatrixLookAtLH(&look, &D3DXVECTOR3(0.0f, 150.0f, -0.1f), &D3DXVECTOR3(0, 0, 0), &D3DXVECTOR3(0, 1, 0));
	D3DXVECTOR4 vColor = D3DXVECTOR4(DirectX::Colors::White);

	inFrustumCnt = 0;
	m_pImmediateContext->RSSetViewports(1, &m_dxRt.m_Viewport);
	m_Map.SetMatrix(&m_Camera["modelview"]->m_matModelRot, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Map.Render(m_pImmediateContext);
	for (size_t i = 0; i < m_matWorld.size(); ++i)
	{
		D3DXMATRIX objWorld;
		D3DXMatrixMultiply(&objWorld, &m_Camera["modelview"]->m_matModelRot, &m_matWorld[i]);
		D3DXVECTOR3 Pos = (D3DXVECTOR3)objWorld.m[3];
		if (m_Frustum.ClassifyPoint(Pos))
		{
			m_boxObj.SetMatrix(&objWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			m_boxObj.Render(m_pImmediateContext);
			++inFrustumCnt;
		}
	}

	m_dxrt1.Begin(m_pImmediateContext, vColor);
	{
		for (size_t i = 0; i < m_matWorld.size(); ++i)
		{
			m_boxObj.SetMatrix(&m_matWorld[i], &look, &m_pMainCamera->m_matProj);
			m_boxObj.Render(m_pImmediateContext);
		}
		m_Frustum.m_box.SetMatrix(nullptr, &look, &m_pMainCamera->m_matProj);
		m_Frustum.Render(m_pImmediateContext);
	}
	m_dxrt1.End(m_pImmediateContext, &m_dxRt);


	D3DXMATRIX dx;
	D3DXMatrixIdentity(&dx);
	m_Plane.SetMatrix(&dx, &dx, &dx);
	m_pImmediateContext->RSSetViewports(1, &m_vp);
	m_Plane.PreRender(m_pImmediateContext);
	m_pImmediateContext->PSSetShaderResources(0, 1, m_dxrt1.m_pShaderResourceView.GetAddressOf());
	m_Plane.PostRender(m_pImmediateContext);
	{
		D2D1_RECT_F rt;
		rt.top = m_vp.TopLeftY;
		rt.left = m_vp.TopLeftX;
		rt.right = m_vp.Width;
		rt.bottom = m_vp.TopLeftY;
		S_DirectWrite.m_pTextFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);
		S_DirectWrite.DrawText(rt, L"Top View", D2D1::ColorF::Black);

		rt.top = (float)g_rtClient.bottom - 30;
		rt.bottom = (float)g_rtClient.bottom + 50;

		std::tstring CountText = L"In Frustum : " + std::to_wstring(inFrustumCnt);
		S_DirectWrite.DrawText(rt, CountText.c_str(), D2D1::ColorF::Black);
	}
//	ID3D11ShaderResourceView* nullshader = nullptr;
//	m_pImmediateContext->PSSetShaderResources(0, 1, &nullshader);

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
