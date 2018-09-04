#include "Sample.h"
#include "Enumeration.h"

Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	return true;
}
bool Sample::Render()
{
	return true;
}
bool Sample::Release()
{
	Enumeration::CreateDeviceInfo(getGIFactory());
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleDXGI");
	wd.Run();
	return 0;
}