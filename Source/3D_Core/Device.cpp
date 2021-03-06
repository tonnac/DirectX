#include "Device.h"

Device::Device() : m_pd3dDevice(nullptr), m_pSwapChain(nullptr), m_pImmediateContext(nullptr), m_pGIFactory(nullptr), m_bFullScreenMode(false)
{
	ZeroMemory(&m_SwapChainDesc, sizeof(DXGI_SWAP_CHAIN_DESC));
}
Device::~Device()
{}

HRESULT Device::CreateDevice()
{
	HRESULT hr;
	UINT createDeviceFlags = D3D11_CREATE_DEVICE_BGRA_SUPPORT; // 넣어줘야 2d와 연동이 가능하다
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
	if (m_pd3dDevice == nullptr) return E_FAIL;
	if (m_pGIFactory != nullptr)
	{
		m_pGIFactory->Release();
		m_pGIFactory = nullptr;
	}
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
HRESULT Device::CreateSwapChain(HWND hWnd, const UINT& iWidth, const UINT& iHeight)
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
	m_SwapChainDesc.Flags = DXGI_SWAP_CHAIN_FLAG_ALLOW_MODE_SWITCH; ////
	if (FAILED(hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &m_SwapChainDesc, &m_pSwapChain)))
	{
		return hr;
	}
	return hr;
}
HRESULT Device::SetRTVDSV()
{
	HRESULT hr;
	ID3D11Texture2D* pBackBuffer;
	hr = m_pSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return false;

	hr = m_dxRt.Create(m_pd3dDevice, g_rtClient.right, g_rtClient.bottom, pBackBuffer);

	return hr;
}
bool Device::CleanupDevice()
{
	m_pSwapChain->SetFullscreenState(false, NULL);
	if (m_pImmediateContext) m_pImmediateContext->ClearState();
	if (m_pSwapChain) m_pSwapChain->Release();
	if (m_pImmediateContext) m_pImmediateContext->Release();
	if (m_pd3dDevice) m_pd3dDevice->Release();
	if (m_pGIFactory) m_pGIFactory->Release();

	m_pd3dDevice = nullptr;
	m_pSwapChain = nullptr;
	m_pImmediateContext = nullptr;
	m_pGIFactory = nullptr;
	return true;
}
void Device::ResizeDevice(const UINT& Width, const UINT& Height)
{
	HRESULT hr;
	if (m_pd3dDevice == nullptr) return;
	m_pImmediateContext->OMSetRenderTargets(0, NULL, NULL);

	// DirectWrite 종속적인 장치 소멸
	DeleteDeviceResources();

	m_dxRt.Reset();

	m_SwapChainDesc.BufferDesc.Height = g_rtClient.right = Height;
	m_SwapChainDesc.BufferDesc.Width = g_rtClient.bottom = Width;

	if (SUCCEEDED(hr = m_pSwapChain->ResizeBuffers(m_SwapChainDesc.BufferCount, m_SwapChainDesc.BufferDesc.Width,
		m_SwapChainDesc.BufferDesc.Height, DXGI_FORMAT_R8G8B8A8_UNORM, m_SwapChainDesc.Flags)))
	{
		DXGI_SWAP_CHAIN_DESC sd;
		m_pSwapChain->GetDesc(&sd);
		m_SwapChainDesc = sd;
	}

	ID3D11Texture2D* pBackbuffer = nullptr;
	m_pSwapChain->GetBuffer(0, IID_PPV_ARGS(&pBackbuffer));
	m_dxRt.Create(m_pd3dDevice, Width, Height, pBackbuffer);

	// DirectWrite 종속적인 장치 생성
	CreateDeviceResources(Width, Height);
}
void Device::DeleteDeviceResources()
{
	return;
}
HRESULT	Device::CreateDeviceResources(const UINT& Width, const UINT& Height)
{
	return S_OK;
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
DXGI_SWAP_CHAIN_DESC Device::getSwapChainDesc() const
{
	return m_SwapChainDesc;
}
BOOL Device::getFullScreenMode() const
{
	return m_bFullScreenMode;
}
void Device::setFullScreenMode(const BOOL& FullScreen)
{
	m_bFullScreenMode = FullScreen;
}