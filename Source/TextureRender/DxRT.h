#pragma once
#include "DxState.h"

class DxRT
{
public:
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> m_pRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> m_pDepthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> m_pTexture = nullptr;

	D3D11_DEPTH_STENCIL_VIEW_DESC m_DepthStencilDesc;
	D3D11_TEXTURE2D_DESC m_TexDesc;
	
	D3D11_VIEWPORT m_Viewport;
	D3D11_VIEWPORT m_OldViewPort;

	ID3D11RenderTargetView* m_pOldRenerTargetView = nullptr;
	ID3D11DepthStencilView* m_pOldDepthStencilView = nullptr;

public:
	void set(ID3D11RenderTargetView* pRSV, ID3D11DepthStencilView* pDSV);

	HRESULT Create(ID3D11Device* pd3dDevice, float width, float height);
	HRESULT CreateDepthStencilView(ID3D11Device* pd3dDevice, UINT width, UINT height);

	bool Begin(ID3D11DeviceContext* pContext, D3DXVECTOR4 vColor);
	bool End(ID3D11DeviceContext* pContext);
};