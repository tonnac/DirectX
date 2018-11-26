#pragma once
#include "DxState.h"

struct DxRT
{

	HRESULT Create(ID3D11Device* pd3dDevice, float width, float height, ID3D11Texture2D* pTexture = nullptr);

	bool Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor);
	bool End(ID3D11DeviceContext* pContext, const DxRT* pDxrt = nullptr);

	void Reset();

	HRESULT CreateDepthStencilView(ID3D11Device* pd3dDevice, UINT width, UINT height);


	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture = nullptr;

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC m_TexDesc;
	
	D3D11_VIEWPORT m_Viewport;
};