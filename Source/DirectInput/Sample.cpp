#include "Sample.h"
#include <cmath>

Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_Dinput.Init();
	return true;
}
bool Sample::Render()
{
	m_Dinput.Frame();
	return true;
}
bool Sample::Release()
{
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleDirectInput");
	wd.Run();
	return 0;
}