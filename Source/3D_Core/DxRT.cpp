#include "DxRT.h"

using Microsoft::WRL::ComPtr;

HRESULT DxRT::Create(ID3D11Device* pd3dDevice, float Width, float Height, ID3D11Texture2D* pTexture)
{
	HRESULT hr = S_OK;

	m_pRenderTargetView.Reset();

	m_Viewport.TopLeftX = 0;
	m_Viewport.TopLeftY = 0;
	m_Viewport.Width = Width;
	m_Viewport.Height = Height;
	m_Viewport.MinDepth = 0.0f;
	m_Viewport.MaxDepth = 1.0f;

	if (pTexture != nullptr)
	{
		hr = pd3dDevice->CreateRenderTargetView(pTexture, nullptr, m_pRenderTargetView.GetAddressOf());
		pTexture->Release();
	}
	else
	{
		m_TexDesc = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_R8G8B8A8_UNORM, Width, Height);

		m_TexDesc.BindFlags |= D3D11_BIND_RENDER_TARGET;
		m_TexDesc.MipLevels = 1;

		hr = pd3dDevice->CreateTexture2D(&m_TexDesc, NULL, m_pTexture.GetAddressOf());
		hr = pd3dDevice->CreateShaderResourceView(m_pTexture.Get(), NULL, m_pShaderResourceView.GetAddressOf());
		hr = pd3dDevice->CreateRenderTargetView(m_pTexture.Get(), NULL, m_pRenderTargetView.GetAddressOf());
	}

	hr = CreateDepthStencilView(pd3dDevice, (UINT)Width, (UINT)Height);
	return hr;
}

HRESULT DxRT::CreateDepthStencilView(ID3D11Device* pd3dDevice, UINT width, UINT height)
{
	HRESULT hr;

	m_pDepthStencilView.Reset();

	ComPtr<ID3D11Texture2D> pDepthStencilTexture = nullptr;

	D3D11_TEXTURE2D_DESC descDepth = CD3D11_TEXTURE2D_DESC(DXGI_FORMAT_D24_UNORM_S8_UINT, width, height);
	descDepth.MipLevels = 1;
	descDepth.BindFlags = D3D11_BIND_DEPTH_STENCIL;

	hr = pd3dDevice->CreateTexture2D(&descDepth, nullptr, pDepthStencilTexture.GetAddressOf());

	D3D11_DEPTH_STENCIL_VIEW_DESC dsvDesc = CD3D11_DEPTH_STENCIL_VIEW_DESC(
		pDepthStencilTexture.Get(),
		D3D11_DSV_DIMENSION_TEXTURE2D,
		descDepth.Format);

	//D3D11_SHADER_RESOURCE_VIEW_DESC srvDesc = CD3D11_SHADER_RESOURCE_VIEW_DESC(
	//	pDepthStencilTexture.Get(),
	//	D3D11_SRV_DIMENSION_UNKNOWN,
	//	DXGI_FORMAT_R24_UNORM_X8_TYPELESS);

	hr = pd3dDevice->CreateDepthStencilView(pDepthStencilTexture.Get(), &dsvDesc,
		m_pDepthStencilView.GetAddressOf());

	m_pDepthStencilView->GetDesc(&m_DepthStencilDesc);

	return hr;
}

bool DxRT::Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor)
{
	ID3D11RenderTargetView* pRender = nullptr;
	pContext->RSSetViewports(1, &m_Viewport);

	pContext->OMSetRenderTargets(1, &pRender, nullptr);
	pContext->OMSetRenderTargets(1, m_pRenderTargetView.GetAddressOf(), m_pDepthStencilView.Get());

	pContext->ClearRenderTargetView(m_pRenderTargetView.Get(), vColor);
	pContext->ClearDepthStencilView(m_pDepthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

	return true;
}

bool DxRT::End(ID3D11DeviceContext* pContext, const DxRT* pDxrt)
{
	if (pDxrt != nullptr)
	{
		pContext->RSSetViewports(1, &pDxrt->m_Viewport);
		ID3D11RenderTargetView* pRender = nullptr;
		pContext->OMSetRenderTargets(1, &pRender, nullptr);
		pContext->OMSetRenderTargets(1, pDxrt->m_pRenderTargetView.GetAddressOf(), pDxrt->m_pDepthStencilView.Get());
	}
	return true;
}

void DxRT::Reset()
{
	m_pDepthStencilView.Reset();
	m_pRenderTargetView.Reset();
}