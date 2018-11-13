#include "DxState.h"

using namespace Microsoft::WRL;

std::array<ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> DxState::m_DSS;
std::array<ComPtr<ID3D11RasterizerState>, (int)E_RSS::Count> DxState::m_RSS;
std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>, (int)E_BSS::Count> DxState::m_BSS;

HRESULT DxState::InitState(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	hr = InitDepthStencliState(pd3dDevice);
	hr = InitRasterizerState(pd3dDevice);
	hr = InitBlendState(pd3dDevice);

	return hr;
}

HRESULT DxState::InitDepthStencliState(ID3D11Device* pd3dDevice)
{
	ComPtr<ID3D11DepthStencilState> DSS;
	D3D11_DEPTH_STENCIL_DESC dsDesc;

	HRESULT hr = S_OK;

	ZeroMemory(&dsDesc, sizeof(D3D11_DEPTH_STENCIL_DESC));
	dsDesc.DepthEnable = TRUE;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

	hr = pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf());
	m_DSS[(int)E_DSS::Default] = DSS;

	DSS.Reset();
	dsDesc.DepthEnable = FALSE;
	hr = pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf());
	m_DSS[(int)E_DSS::Disable] = DSS;

	DSS.Reset();
	dsDesc.DepthFunc = D3D11_COMPARISON_GREATER;
	hr = pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf());
	m_DSS[(int)E_DSS::Greater] = DSS;

	return hr;
}

HRESULT DxState::InitRasterizerState(ID3D11Device* pd3dDevice)
{
	ComPtr<ID3D11RasterizerState> RSS;
	D3D11_RASTERIZER_DESC rsDesc;
	
	HRESULT hr = S_OK;

	ZeroMemory(&rsDesc, sizeof(D3D11_RASTERIZER_DESC));
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_NONE;
	rsDesc.DepthClipEnable = TRUE;


	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::Default] = RSS;

	RSS.Reset();
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::Wireframe] = RSS;

	RSS.Reset();
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::Solidfront] = RSS;

	RSS.Reset();
	rsDesc.CullMode = D3D11_CULL_BACK;
	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::SolidBack] = RSS;

	return hr;
}

HRESULT DxState::InitBlendState(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;

	ComPtr<ID3D11BlendState> BSS;
	D3D11_BLEND_DESC bsDesc;
	ZeroMemory(&bsDesc, sizeof(D3D11_BLEND_DESC));
	bsDesc.RenderTarget[0].BlendEnable = TRUE;

	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bsDesc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;

	bsDesc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_SRC_ALPHA;
	bsDesc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_INV_SRC_ALPHA;
	bsDesc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bsDesc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	pd3dDevice->CreateBlendState(&bsDesc, BSS.GetAddressOf());
	m_BSS[(int)E_BSS::Default] = BSS;

	BSS.Reset();
	bsDesc.RenderTarget[0].SrcBlend = D3D11_BLEND_ZERO;
	bsDesc.RenderTarget[0].DestBlend = D3D11_BLEND_DEST_ALPHA;
	pd3dDevice->CreateBlendState(&bsDesc, BSS.GetAddressOf());
	m_BSS[(int)E_BSS::No] = BSS;

	return hr;
}