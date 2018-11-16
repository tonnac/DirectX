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
	m_boxObj.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_boxObj2.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_boxObj3.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_plane.Create(m_pd3dDevice, L"shape.hlsl", L"../../data/effect/drain1.dds");
	m_line.Create(m_pd3dDevice, L"shape.hlsl");
	m_pMainCamera->m_vPos = D3DXVECTOR3(5.0f, 5.0f, -25.0f);
	return true;
}
bool Sample::Frame()
{
	m_boxObj.Frame();
	m_plane.Frame();
	return true;
}
bool Sample::Render()
{
	static float dll = 0.0f;
	dll += (float)(0.15f * D3DX_PI) * g_fSecPerFrame;
	

	//D3DXMatrixRotationAxis(&m_boxObj.m_SRTMat.matSpinRotation, &D3DXVECTOR3(cosf(g_fGameTimer), cosf(g_fGameTimer), sinf(g_fGameTimer)), dll);
	D3DXMatrixRotationYawPitchRoll(&m_boxObj.m_SRTMat.matSpinRotation, dll, dll * 2, dll*1.5f);

	m_boxObj.m_SRTMat.matWorld = m_boxObj.m_SRTMat.matSpinRotation;

	D3DXMatrixTranslation(&m_plane.m_SRTMat.matTranslate, 4, 0, 0.0f);
	D3DXMatrixRotationY(&m_plane.m_SRTMat.matSpinRotation, dll);
	D3DXMatrixRotationY(&m_plane.m_SRTMat.matOrbitRotation, dll * 2.0f);

	m_plane.m_SRTMat.matWorld =
		m_plane.m_SRTMat.matSpinRotation *
		m_plane.m_SRTMat.matTranslate *
		m_plane.m_SRTMat.matOrbitRotation *

		m_boxObj.m_SRTMat.matSpinRotation;


	D3DXMatrixTranslation(&m_boxObj2.m_SRTMat.matTranslate, 0, 4, 0.0f);
	D3DXMatrixScaling(&m_boxObj2.m_SRTMat.matScale, 0.75f, 0.75f, 0.75f);
	D3DXMatrixRotationY(&m_boxObj2.m_SRTMat.matSpinRotation, dll);
	D3DXMatrixRotationX(&m_boxObj2.m_SRTMat.matOrbitRotation, dll * 3.0f);
	m_boxObj2.m_SRTMat.matWorld =
		m_boxObj2.m_SRTMat.matScale *
		m_boxObj2.m_SRTMat.matSpinRotation *
		m_boxObj2.m_SRTMat.matTranslate *
		m_boxObj2.m_SRTMat.matOrbitRotation *

		m_plane.m_SRTMat.matSpinRotation *
		m_plane.m_SRTMat.matTranslate *
		m_plane.m_SRTMat.matOrbitRotation *

		m_boxObj.m_SRTMat.matSpinRotation;

	D3DXMatrixTranslation(&m_boxObj3.m_SRTMat.matTranslate, 4, 0, 0.0f);
	D3DXMatrixScaling(&m_boxObj3.m_SRTMat.matScale, 0.75f, 0.75f, 0.75f);
	D3DXMatrixRotationZ(&m_boxObj3.m_SRTMat.matSpinRotation, dll);
	D3DXMatrixRotationY(&m_boxObj3.m_SRTMat.matOrbitRotation, dll * 4.0f);
	m_boxObj3.m_SRTMat.matWorld =
		m_boxObj3.m_SRTMat.matScale *
		m_boxObj3.m_SRTMat.matSpinRotation *
		m_boxObj3.m_SRTMat.matTranslate *
		m_boxObj3.m_SRTMat.matOrbitRotation *

		m_boxObj2.m_SRTMat.matScale *
		m_boxObj2.m_SRTMat.matSpinRotation *
		m_boxObj2.m_SRTMat.matTranslate *
		m_boxObj2.m_SRTMat.matOrbitRotation *

		m_plane.m_SRTMat.matSpinRotation *
		m_plane.m_SRTMat.matTranslate *
		m_plane.m_SRTMat.matOrbitRotation *

		m_boxObj.m_SRTMat.matSpinRotation;

	m_boxObj.SetMatrix(&m_boxObj.m_SRTMat.matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_boxObj2.SetMatrix(&m_boxObj2.m_SRTMat.matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_boxObj3.SetMatrix(&m_boxObj3.m_SRTMat.matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_plane.SetMatrix(&m_plane.m_SRTMat.matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_line.SetMatrix(&m_line.m_SRTMat.matWorld, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);

	m_boxObj.Render(m_pImmediateContext);
	m_plane.Render(m_pImmediateContext);
	m_boxObj2.Render(m_pImmediateContext);
	m_boxObj3.Render(m_pImmediateContext);

	m_line.Draw(m_pImmediateContext, m_boxObj.m_vPosition, m_plane.m_vPosition, D3DXVECTOR4(DirectX::Colors::Black));
	m_line.Draw(m_pImmediateContext, m_plane.m_vPosition, m_boxObj2.m_vPosition, D3DXVECTOR4(DirectX::Colors::Black));
	m_line.Draw(m_pImmediateContext, m_boxObj2.m_vPosition, m_boxObj3.m_vPosition, D3DXVECTOR4(DirectX::Colors::Black));

	m_line.Draw(m_pImmediateContext, m_plane.m_vPosition, m_plane.m_vLook * 1000, D3DXVECTOR4(DirectX::Colors::Red));
	m_line.Draw(m_pImmediateContext, m_plane.m_vPosition, m_plane.m_vSide * 1000, D3DXVECTOR4(DirectX::Colors::Blue));
	m_line.Draw(m_pImmediateContext, m_plane.m_vPosition, m_plane.m_vUp * 1000, D3DXVECTOR4(DirectX::Colors::Green));
	return true;
}
bool Sample::Release()
{
	m_boxObj.Release();
	m_plane.Release();
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

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
