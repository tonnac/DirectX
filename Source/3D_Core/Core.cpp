#include "Core.h"
bool Core::GameInit()
{
	Init();
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateSwapChain(m_hWnd, m_iWindowWidth, m_iWindowHeight))) return false;
	if (FAILED(SetRendetTargetView())) return false;
	if (FAILED(SetViewPort())) return false;
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
	Release();
	if (CleanupDevice() == false) return false;
	return true;
}
bool Core::GameFrame()
{
	Frame();
	return true;
}
bool Core::GameRender()
{
	if (GamePreRender() == false) return false;
	Render();
	if (GamePostRender() == false) return false;
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

bool Core::GamePreRender()
{
	m_pImmediateContext->OMSetRenderTargets(1, &m_pRenderTargetView, NULL);
	float ClearColor[] = { 1.0f, 1.0f, 0.0f, 1.0f };
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, ClearColor);
	m_pSwapChain->Present(0, 0);
	return true;
}
bool Core::GamePostRender()
{
	return true;
}
