#include "Core.h"
bool Core::GameInit()
{
#pragma region
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateSwapChain(m_hWnd, m_iWindowWidth, m_iWindowHeight))) return false;
	if (FAILED(SetRendetTargetView())) return false;
	SetViewPort();
#pragma endregion Device_Init

#ifdef DEVICE_INFO
	Enumeration::CreateDeviceInfo(getGIFactory());
#endif

#pragma region 
	IDXGISurface1 * pSurface = nullptr;
	IDXGISwapChain* pSwapChain = getSwapChain();
	pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pSurface);
	S_DirectWrite.Init();
	S_DirectWrite.Set(g_rtClient.right, g_rtClient.bottom, pSurface);
	pSurface->Release();
#pragma endregion Write_Init

	IDXGIFactory * pFactory = getGIFactory();
	pFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES); // 윈도우 메시지와 ALT+ENTER로 인한 전체화면을 막음
	m_Timer.Init();
	Init();
	return true;
}
bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}
bool Core::GameRelease()
{
	m_Timer.Release();
	Release();
	if (CleanupDevice() == false) return false;
	return true;
}
bool Core::GameFrame()
{
	m_Timer.Frame();
	Frame();
	return true;
}
bool Core::GameRender()
{
	if (PreRender() == false) return false;
	Render();
	m_Timer.Render();
	if (PostRender() == false) return false;
	return true;
}
bool Core::PreInit()
{
	return true;
}
void Core::DeleteDeviceResources()
{
	S_DirectWrite.DiscardDeviceResources();
}
HRESULT Core::CreateDeviceResources(const UINT& Width, const UINT& Height)
{
	HRESULT hr;
	IDXGISurface1 * pBackBuffer = nullptr;
	IDXGISwapChain * pSwapChain = getSwapChain();
	hr = pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return false;
	S_DirectWrite.CreateDeviceResources(pBackBuffer);
	pBackBuffer->Release();
	return hr;
}
bool Core::Init() { return true; }
bool Core::Frame() { return true; }
bool Core::Render() { return true; }
bool Core::Release() { return true; }
bool Core::PreRender()
{
	ID3D11DeviceContext* pContext = getContext();
	ID3D11RenderTargetView* pRenderTargetView = getRenderTargetView();
	D3D11_VIEWPORT& ViewPort = getViewPort();
	float ClearColor[] = { 0.25f, 0.25f, 0.25f, 1.0f };
	pContext->ClearRenderTargetView(pRenderTargetView, ClearColor);
	return true;
}
bool Core::PostRender()
{
	IDXGISwapChain* pSwapChain = getSwapChain();
	pSwapChain->Present(0, 0);
	return true;
}
