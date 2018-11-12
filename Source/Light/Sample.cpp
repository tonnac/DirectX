#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <DirectXColors.h>


Sample::Sample()
{}
Sample::~Sample()
{}

D3DXVECTOR3 pos(float radius, float theta, float phi)
{
	float x, y, z;
	y = radius * cosf(phi);
	x = radius * sinf(phi) * cosf(theta);
	z = radius * sinf(phi) * sinf(theta);

	return D3DXVECTOR3(x, y, z);
}

bool Sample::Init()
{
	DxState::InitState(m_pd3dDevice);

	D3D11_RASTERIZER_DESC rasDesc;
	ZeroMemory(&rasDesc, sizeof(D3D11_RASTERIZER_DESC));
	rasDesc.FillMode = D3D11_FILL_WIREFRAME;
	rasDesc.CullMode = D3D11_CULL_NONE;
	rasDesc.DepthClipEnable = TRUE;
	
	{
		mEyePos = { 0.0f, 20.0f, -25.0f };
		mMaterial.DiffuseAlbedo = D3DXVECTOR4(DirectX::Colors::LightSteelBlue);
		mMaterial.FresnelR0 = D3DXVECTOR3(0.2f, 0.2f, 0.2f);
		mMaterial.Roughness = 0.125f;
	}

	D3DXMatrixIdentity(&m_matWorld[0]);
	D3DXMatrixIdentity(&m_matWorld[1]);
	D3DXMatrixLookAtLH(&m_matView, 
		&mEyePos,
		&D3DXVECTOR3(+0.0f, +0.0f, +0.0f), 
		&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);

	m_sphere.Set(m_pd3dDevice, 2, 30, 30);
	return true;
}
bool Sample::Frame()
{
	m_sphere.Frame();
	return true;
}
bool Sample::Render()
{
	static float dll = 0.0f;
	dll += (float)(0.25f * D3DX_PI) * g_fSecPerFrame;


	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)E_DSS::Default].Get(), 0);

	D3DXMatrixTranspose(&m_sphere.m_cbData.matWorld, &m_matWorld[0]);
	D3DXMatrixTranspose(&m_sphere.m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_sphere.m_cbData.matProj, &m_matProj);

	m_sphere.m_cbData.AmbientLight = { 0.15f, 0.15f, 0.15f, 1.0f };
	m_sphere.m_cbData.DiffuseAlbedo = mMaterial.DiffuseAlbedo;
	m_sphere.m_cbData.FresnelR0 = mMaterial.FresnelR0;
	m_sphere.m_cbData.Roughness = mMaterial.Roughness;

	m_sphere.m_cbData.EyePosW = mEyePos;
	m_sphere.m_cbData.Lights[0].Direction = -pos(1.0f, (-(float)D3DX_PI / 4) + dll, (float)D3DX_PI / 3);
	m_sphere.m_cbData.Lights[0].Strength = { 0.6f, 0.6f, 0.6f };

	m_sphere.m_cbData.Lights[1].Direction = -pos(1.0f, (float)D3DX_PI + dll, (float)D3DX_PI / 3);
	m_sphere.m_cbData.Lights[1].Strength = { 0.3f, 0.3f, 0.3f };

	m_sphere.m_cbData.Lights[2].Direction = -pos(1.0f, (float)D3DX_PI / 2 + dll, (float)D3DX_PI / 3);
	m_sphere.m_cbData.Lights[2].Strength = { 0.1f, 0.1f, 0.1f };

	m_sphere.Render(m_pImmediateContext);

	D3DXMATRIX scale;
	D3DXMATRIX tras;
	D3DXMATRIX orbit;
	D3DXMatrixScaling(&scale, 0.6f, 0.6f, 0.6f);
	D3DXMatrixTranslation(&tras, 3.0f, 0.0f, 3.0f);
	m_matWorld[1] = scale * tras;
	D3DXMatrixRotationY(&orbit, dll * 0.5f);
	m_matWorld[1] = m_matWorld[1] * orbit;
	D3DXMatrixTranspose(&m_sphere.m_cbData.matWorld, &m_matWorld[1]);

	m_sphere.Render(m_pImmediateContext);

	return true;
}
bool Sample::Release()
{
	m_sphere.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
