#include "DxState.h"

using namespace Microsoft::WRL;

std::array<ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> DxState::m_DSS;
std::array<ComPtr<ID3D11RasterizerState>, (int)E_RSS::Count> DxState::m_RSS;

HRESULT DxState::InitState(ID3D11Device* pd3dDevice)
{
	HRESULT hr = S_OK;
	hr = InitDepthStencliState(pd3dDevice);
	hr = InitRasterizerState(pd3dDevice);

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

	DSS.ReleaseAndGetAddressOf();
	dsDesc.DepthEnable = FALSE;
	hr = pd3dDevice->CreateDepthStencilState(&dsDesc, DSS.GetAddressOf());
	m_DSS[(int)E_DSS::Disable] = DSS;

	DSS.ReleaseAndGetAddressOf();
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

	RSS.ReleaseAndGetAddressOf();
	rsDesc.FillMode = D3D11_FILL_WIREFRAME;
	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::Wireframe] = RSS;

	RSS.ReleaseAndGetAddressOf();
	rsDesc.FillMode = D3D11_FILL_SOLID;
	rsDesc.CullMode = D3D11_CULL_FRONT;
	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::Solidfront] = RSS;

	RSS.ReleaseAndGetAddressOf();
	rsDesc.CullMode = D3D11_CULL_BACK;
	hr = pd3dDevice->CreateRasterizerState(&rsDesc, RSS.GetAddressOf());
	m_RSS[(int)E_RSS::SolidBack] = RSS;

	return hr;
}