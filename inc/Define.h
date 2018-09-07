#pragma once
#define DIRECTINPUT_VERSION 0x800
#include <windows.h>
#include <cassert>
#include <tchar.h>
#include <D3D11.h>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <map>
#include <sstream>
#include <iomanip>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "3D_Core.lib")

#define KEYSTATECOUNT 256
#define SAMPLE_BUFFER_SIZE 16

#ifndef V
#define V(x) {hr = (x);}
#endif

#ifndef V_RETURN
#define V_RETURN(x) {hr = (x); if(FAILED(hr)) {return hr;}}
#endif


namespace std
{
	using tstring = std::basic_string<TCHAR>;

	using tofstream = std::basic_ifstream<TCHAR>;
	using tifstream = std::basic_ofstream<TCHAR>;
	using tfstream = std::basic_fstream<TCHAR>;

	using tsstream = std::basic_stringstream<TCHAR>;
}

extern HWND			g_hWnd;
extern HINSTANCE	g_hInstance;
extern RECT			g_rtClient;
extern BOOL			g_bActiveApp;
extern FLOAT		g_fGameTimer;
extern FLOAT		g_fSecPerFrame;

template <class K>
class Singleton
{
protected:
	Singleton() {};
public:
	static K & getInstance()
	{
		static K inst;
		return inst;
	}
};