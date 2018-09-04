#include "Device.h"

Device::Device() : m_pd3dDevice(nullptr), m_pSwapChain(nullptr), m_pRenderTargetView(nullptr),
m_pImmediateContext(nullptr), m_pGIFactory(nullptr)
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
}
Device::~Device()
{}

HRESULT Device::CreateDevice()
{
	HRESULT hr;
	UINT createDeviceFlags = 0;
#ifdef _DEBUG
	createDeviceFlags |= D3D11_CREATE_DEVICE_DEBUG;
#endif

	D3D_DRIVER_TYPE driverTypes[] =
	{
		D3D_DRIVER_TYPE_HARDWARE,
		D3D_DRIVER_TYPE_WARP,
		D3D_DRIVER_TYPE_SOFTWARE
	};
	UINT numDriverTypes = sizeof(driverTypes) / sizeof(D3D_DRIVER_TYPE);

	D3D_FEATURE_LEVEL featureLevels[] =
	{
		D3D_FEATURE_LEVEL_11_0,
		D3D_FEATURE_LEVEL_10_1,
		D3D_FEATURE_LEVEL_10_0
	};
	UINT numFeatureLevels = sizeof(featureLevels) / sizeof(D3D_FEATURE_LEVEL);
	for (UINT driverTypeIndex = 0; driverTypeIndex < numDriverTypes; ++driverTypeIndex)
	{
		m_DriverType = driverTypes[driverTypeIndex];
		if (FAILED(hr = D3D11CreateDevice(NULL, m_DriverType, NULL, createDeviceFlags, featureLevels, numFeatureLevels,
			D3D11_SDK_VERSION, &m_pd3dDevice, &m_FeatureLevel, &m_pImmediateContext)))
		{
			continue;
		}
		break;
	}
	if (FAILED(hr))
	{
		return false;
	}
	return S_OK;
}
HRESULT Device::CreateGIFactory()
{
	if (m_pd3dDevice == NULL) return E_FAIL;
	HRESULT hr;// = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory) );
	IDXGIDevice * pDXGIDevice;
	hr = m_pd3dDevice->QueryInterface(__uuidof(IDXGIDevice), (void **)&pDXGIDevice);

	IDXGIAdapter * pDXGIAdapter;
	hr = pDXGIDevice->GetParent(__uuidof(IDXGIAdapter), (void **)&pDXGIAdapter);

	pDXGIAdapter->GetParent(__uuidof(IDXGIFactory), (void **)&m_pGIFactory);

	pDXGIDevice->Release();
	pDXGIAdapter->Release();
	return hr;
}
HRESULT Device::CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == nullptr) return S_FALSE;
	m_SwapChainDesc.BufferCount = 1;
	m_SwapChainDesc.BufferDesc.Width = iWidth;
	m_SwapChainDesc.BufferDesc.Height = iHeight;
	m_SwapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	m_SwapChainDesc.BufferDesc.RefreshRate.Numerator = 60;
	m_SwapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
	m_SwapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	m_SwapChainDesc.OutputWindow = g_hWnd;
	m_SwapChainDesc.SampleDesc.Count = 1;
	m_SwapChainDesc.SampleDesc.Quality = 0;
	m_SwapChainDesc.Windowed = TRUE;
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH;
	if (FAILED(hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &m_SwapChainDesc, &m_pSwapChain)))
	{
		return hr;
	}
	return hr;
}
HRESULT Device::SetRendetTargetView()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return false;

	hr = m_pd3dDevice->CreateRenderTargetView(pBackBuffer, NULL, &m_pRenderTargetView);
	pBackBuffer->Release();

	if (FAILED(hr)) return false;

	return hr;
}
void Device::SetViewPort()
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC Desc;
	if (FAILED(hr = m_pSwapChain->GetDesc(&Desc)))
	{
		return;
	}
	m_ViewPort.Width = static_cast<FLOAT>(Desc.BufferDesc.Width);
	m_ViewPort.Height = static_cast<FLOAT>(Desc.BufferDesc.Height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);

	g_rtClient.right = m_SwapChainDesc.BufferDesc.Width;
	g_rtClient.bottom = m_SwapChainDesc.BufferDesc.Height;


}
bool Device::CleanupDevice()
{
//	m_pSwapChain->SetFullscreenState(!isFullMode, NULL);
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pRenderTargetView) m_pRenderTargetView->Release();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pGIFactory) m_pGIFactory->Release();

	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pRenderTargetView = nullptr;
	m_pImmediateContext = nullptr;
	m_pGIFactory = nullptr;
	return true;
}
void Device::ResizeDevice(UINT Width, UINT Height)
{
	HRESULT hr;
	if (m_pd3dDevice == nullptr) return;
	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);
	m_pRenderTargetView->Release();

	m_SwapChainDesc.BufferDesc.Height = Height;
	m_SwapChainDesc.BufferDesc.Width = Width;

	if (SUCCEEDED(hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, m_SwapChainDesc.BufferDesc.Width,
		m_SwapChainDesc.BufferDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, m_SwapChainDesc.Flags)))
	{
		DXGI_SWAP_CHAIN_DESC sd;
		m_pSwapChain->GetDesc(&sd);
	}

	if (FAILED(SetRendetTargetView())) return;
	SetViewPort();
}
ID3D11Device *	Device::getDevice() const
{
	if (m_pd3dDevice)
		return m_pd3dDevice;
	return nullptr;
}
ID3D11DeviceContext* Device::getContext() const
{
	if (m_pImmediateContext)
		return m_pImmediateContext;
	return nullptr;
}
IDXGIFactory* Device::getGIFactory() const
{
	if (m_pGIFactory)
		return m_pGIFactory;
	return nullptr;
}
IDXGISwapChain* Device::getSwapChain() const
{
	if (m_pSwapChain)
		return m_pSwapChain;
	return nullptr;
}
ID3D11RenderTargetView*	Device::getRenderTargetView() const
{
	if (m_pRenderTargetView)
		return m_pRenderTargetView;
	return nullptr;
}
DXGI_SWAP_CHAIN_DESC Device::getSwapChainDesc() const
{
	return m_SwapChainDesc;
}