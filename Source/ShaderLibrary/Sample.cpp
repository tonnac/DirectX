#include "Sample.h"
#include "DirectInput.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_Object.Create(getDevice(), L"EPE", L"VertexShader.txt", L"../../data/effect/Particle6.dds");
	return true;
}
bool Sample::Frame()
{
	m_Object.Frame();
	return true;
}
bool Sample::Render()
{
	m_Object.Render(getContext());
	return true;
}
bool Sample::Release()
{
	m_Object.Release();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleVertexBuffer");
	wd.Run();
	return 0;
}
