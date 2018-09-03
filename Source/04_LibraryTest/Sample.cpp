#include "Sample.h"

Sample::Sample()
{}
Sample::~Sample()
{}
bool Sample::Init()
{
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateSwapChain(m_hWnd, m_iWindowWidth, m_iWindowHeight))) return false;
	if (FAILED(SetRendetTargetView())) return false;
	if (FAILED(SetViewPort())) return false;
	return true;
}
bool Sample::Render()
{
	if (PreRender() == false) return false;
	if (PostRender() == false) return false;
	return true;
}
bool Sample::Release()
{
	if (CleanupDevice() == false) return false;
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"Create Device");
	wd.Run();
	return 0;
}