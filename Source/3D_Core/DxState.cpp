#include "DxState.h"

using namespace Microsoft::WRL;

std::array<ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> DxState::m_DSS;

HRESULT DxState::SetState(ID3D11Device* pd3dDevice)
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

	return hr;
}