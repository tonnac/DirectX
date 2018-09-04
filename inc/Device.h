#pragma once
#include "Define.h"

class Device
{
public:
	Device();
	~Device();
public:
	HRESULT					CreateDevice();
	HRESULT					CreateGIFactory();
	HRESULT					CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight);
	HRESULT					SetRendetTargetView();
	void					SetViewPort();
	bool					CleanupDevice();
public:
	void					ResizeDevice(UINT Width, UINT Height);
public:
	ID3D11Device *			getDevice() const;
	ID3D11DeviceContext*	getContext() const;
	IDXGIFactory*			getGIFactory() const;
	IDXGISwapChain*			getSwapChain() const;
	ID3D11RenderTargetView*	getRenderTargetView() const;
	DXGI_SWAP_CHAIN_DESC	getSwapChainDesc() const;
private:
	ID3D11Device *			m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	IDXGIFactory*			m_pGIFactory;

	IDXGISwapChain*			m_pSwapChain;

	ID3D11RenderTargetView* m_pRenderTargetView;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D_DRIVER_TYPE			m_DriverType;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
	D3D11_VIEWPORT			m_ViewPort;
};