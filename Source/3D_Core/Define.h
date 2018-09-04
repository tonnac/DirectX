#pragma once
#include <windows.h>
#include <cassert>
#include <tchar.h>
#include <D3D11.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "3D_Core.lib")

extern HWND g_hWnd;
extern RECT g_rtClient;
extern BOOL g_bActiveApp;