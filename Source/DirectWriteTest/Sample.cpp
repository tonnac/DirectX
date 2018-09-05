#include "Sample.h"
#include <cmath>

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
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleDirectWrite");
	wd.Run();
	return 0;
}