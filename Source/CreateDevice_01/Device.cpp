#include "Device.h"

Device::Device() : m_pd3dDevice(nullptr), m_pSwapChain(nullptr), m_pRenderTargetView(nullptr),
m_pImmediateContext(nullptr), m_pGIFactory(nullptr)
{}
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
			DXTRACE_ERR(DXGetErrorDescription(hr), hr);
			continue;
		}
		break;
	}
	if (FAILED(hr))
	{
		DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr);
		return false;
	}
	DXTRACE_MSG(DXGetErrorDescription(hr));
	return S_OK;
}
HRESULT Device::CreateGIFactory()
{
	HRESULT hr;
	if (FAILED(hr = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)(&m_pGIFactory))))
	{
		DXTRACE_ERR_MSGBOX(DXGetErrorDescription(hr), hr);
		return false;
	}
	return hr;
}
HRESULT Device::CreateSwapChain(HWND hWnd, UINT iWidth, UINT iHeight)
{
	HRESULT hr = S_OK;
	if (m_pGIFactory == nullptr) return S_FALSE;
	DXGI_SWAP_CHAIN_DESC sd;
	ZeroMemory(&sd, sizeof(DXGI_SWAP_CHAIN_DESC));
	sd.BufferCount = 1;
	sd.BufferDesc.Width = iWidth;
	sd.BufferDesc.Height = iHeight;
	sd.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	sd.BufferDesc.RefreshRate.Numerator = 60;
	sd.BufferDesc.RefreshRate.Denominator = 1;
	sd.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	sd.OutputWindow = g_hWnd;
	sd.SampleDesc.Count = 1;
	sd.SampleDesc.Quality = 0;
	sd.Windowed = TRUE;
	if (FAILED(hr = m_pGIFactory->CreateSwapChain(m_pd3dDevice, &sd, &m_pSwapChain)))
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
HRESULT Device::SetViewPort()
{
	HRESULT hr;
	DXGI_SWAP_CHAIN_DESC Desc;
	if (FAILED(hr = m_pSwapChain->GetDesc(&Desc)))
	{
		return hr;
	}
	m_ViewPort.Width = static_cast<FLOAT>(Desc.BufferDesc.Width);
	m_ViewPort.Height = static_cast<FLOAT>(Desc.BufferDesc.Height);
	m_ViewPort.MinDepth = 0.0f;
	m_ViewPort.MaxDepth = 1.0f;
	m_ViewPort.TopLeftX = 0;
	m_ViewPort.TopLeftY = 0;
	m_pImmediateContext->RSSetViewports(1, &m_ViewPort);
	return hr;
}
bool Device::CleanupDevice()
{
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
bool Device::PreRender()
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	float ClearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pSwapChain->Present(0, 0);
	return true;
}
bool Device::PostRender()
{
	return true;
}
