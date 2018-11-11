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
	HRESULT					CreateSwapChain(HWND hWnd, const UINT& iWidth, const UINT& iHeight);
	HRESULT					SetRTVDSV();
	void					SetViewPort();
	bool					CleanupDevice();
public:
	void					ResizeDevice(const UINT& Width, const UINT& Height);
	//Direct Write ¼Ò¸ê & »ý¼º
	virtual void			DeleteDeviceResources();
	virtual HRESULT			CreateDeviceResources(const UINT& Width, const UINT& Height);
public:
	ID3D11Device *			getDevice() const;
	ID3D11DeviceContext*	getContext() const;
	IDXGIFactory*			getGIFactory() const;
	IDXGISwapChain*			getSwapChain() const;
	ID3D11RenderTargetView*	getRenderTargetView() const;
	D3D11_VIEWPORT&			getViewPort();
	DXGI_SWAP_CHAIN_DESC	getSwapChainDesc() const;
	BOOL					getFullScreenMode() const;
public:
	void					setFullScreenMode(const BOOL& FullScreen);
protected:
	ID3D11Device *			m_pd3dDevice;
	ID3D11DeviceContext*	m_pImmediateContext;

	IDXGIFactory*			m_pGIFactory;

	IDXGISwapChain*			m_pSwapChain;

	ID3D11RenderTargetView* m_pRenderTargetView;
	ID3D11DepthStencilView* m_pDepthStencilView;
	D3D_FEATURE_LEVEL		m_FeatureLevel;
	D3D_DRIVER_TYPE			m_DriverType;
	DXGI_SWAP_CHAIN_DESC	m_SwapChainDesc;
	D3D11_VIEWPORT			m_ViewPort;

	BOOL					m_bFullScreenMode;
};