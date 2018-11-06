#include "Sample.h"
#include "DirectInput.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_Obj.Init();
	m_Obj.Set(getDevice());
	return true;
}
bool Sample::Frame()
{
	m_Obj.Frame();
	return true;
}
bool Sample::Render()
{
	m_Obj.Render(getContext());
	return true;
}
bool Sample::Release()
{
	m_Obj.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
