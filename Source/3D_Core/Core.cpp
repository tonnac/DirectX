#include "Core.h"
bool Core::GameInit()
{
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateSwapChain(m_hWnd, m_iWindowWidth, m_iWindowHeight))) return false;
	if (FAILED(SetRendetTargetView())) return false;
	SetViewPort();

	
#ifdef DEVICE_INFO
	Enumeration::CreateDeviceInfo(getGIFactory());
#endif
	IDXGIFactory * pFactory = getGIFactory();
	pFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES); // 윈도우 메시지와 ALT+ENTER로 인한 전체화면을 막음

	IDXGISurface1 * pSurface = nullptr;
	IDXGISwapChain* pSwapChain = getSwapChain();

	pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pSurface);

	m_Timer.Init();
	S_DirectWrite.Init();

	S_DirectWrite.Set(g_rtClient.right, g_rtClient.bottom, pSurface);
	pSurface->Release();
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
	m_Timer.Render();
	Render();
	if (PostRender() == false) return false;
	return true;
}
bool Core::PreInit()
{
	return true;
}
bool Core::Init() { return true; }
bool Core::Frame() { return true; }
bool Core::Render() { return true; }
bool Core::Release() { return true; }
bool Core::PreRender()
{
	ID3D11DeviceContext* pContext = getContext();
	ID3D11RenderTargetView* pRenderTargetView = getRenderTargetView();
	IDXGISwapChain* pSwapChain = getSwapChain();
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
