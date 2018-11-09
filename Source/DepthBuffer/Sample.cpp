#include "Sample.h"
#include "DirectInput.h"


Sample::Sample()
{}
Sample::~Sample()
{}

HRESULT Sample::CreateDSV()
{
	HRESULT hr = S_OK;
	ID3D11Device* pDevice = getDevice();

	ID3D11Texture2D* pTex;
	D3D11_TEXTURE2D_DESC td;
	td.Width = g_rtClient.right;
	td.Height = g_rtClient.bottom;
	td.MipLevels = 1;
	td.ArraySize = 1;
	td.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	td.SampleDesc.Count = 1;
	td.SampleDesc.Quality = 0;
	td.Usage = D3D11_USAGE_DEFAULT;
	td.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	td.CPUAccessFlags = 0;
	td.MiscFlags = 0;
	hr = pDevice->CreateTexture2D(&td, nullptr, &pTex);

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvd;
	dsvd.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	dsvd.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	dsvd.Flags = 0;
	dsvd.Texture2D.MipSlice = 0;
	hr = pDevice->CreateDepthStencilView(
		pTex,
		&dsvd,
		&m_pDSV);

	D3D11_DEPTH_STENCIL_DESC dsd;
	ZeroMemory(&dsd, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsd.DepthEnable = TRUE;
	dsd.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsd.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;
	hr = pDevice->CreateDepthStencilState(&dsd,	&m_pDSVStateEnable);

	dsd.DepthFunc = D3D11_COMPARISON_GREATER;
	hr = pDevice->CreateDepthStencilState(&dsd, &m_pDSVStateDisable);
	return hr;
}

bool Sample::Init()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixLookAtLH(&m_matView, 
		&D3DXVECTOR3(+0.0f, +0.0f, -5.0f), 
		&D3DXVECTOR3(+0.0f, +0.0f, +0.0f), 
		&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));
	D3DXMatrixPerspectiveFovLH(&m_matProj, (float)D3DX_PI * 0.25f, g_rtClient.right / (float)g_rtClient.bottom, 1.0f, 100.0f);


	CreateDSV();
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
	m_Obj.m_cbData.vColor = D3DXVECTOR4(0.4579f, 0.32879f, 0.87546f, 1.0f);
	m_matWorld._43 = 1.0f;

	D3DXMatrixLookAtLH(&m_matView,
		&D3DXVECTOR3(+0.0f, +0.0f, -80.0f * (cosf(g_fGameTimer) * 0.5f + 0.5f)),
		&D3DXVECTOR3(+0.0f, +0.0f, +0.0f),
		&D3DXVECTOR3(+0.0f, +1.0f, +0.0f));

	D3DXMATRIX ll;
	D3DXMatrixRotationZ(&ll, g_fGameTimer);
	m_matView = ll * m_matView;

	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matProj, &m_matProj);

	m_pImmediateContext->ClearDepthStencilView(m_pDSV, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, m_pDSV);
	m_pImmediateContext->OMSetDepthStencilState(m_pDSVStateEnable, 0);

	m_Obj.Render(m_pImmediateContext);

	m_pImmediateContext->OMSetDepthStencilState(m_pDSVStateDisable, 0);

	m_matWorld._43 = 3.0f;
	m_Obj.m_cbData.vColor = D3DXVECTOR4(0.35798f, 0.57897f, 0.13247f, 1.0f);
	D3DXMatrixTranspose(&m_Obj.m_cbData.matWorld, &m_matWorld);

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
