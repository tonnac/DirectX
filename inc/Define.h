#pragma once
#pragma warning( disable : 4005)
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
#include <list>
#include <algorithm>
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <array>


#define PI 3.141592
#define DegreeToRadian(x) CASTING(FLOAT,(x) / 180 * PI)

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "3D_Core.lib")
#pragma comment(lib, "d3dx11.lib")

#define KEYSTATECOUNT 256
#define SAMPLE_BUFFER_SIZE 16
#define CASTING(x, y) static_cast<x>((y)) 

#ifndef V
#define V(x) {hr = (x);}
#endif

#ifndef V_RETURN
#define V_RETURN(x) {hr = (x); if(FAILED(hr)) {return hr;}}
#endif

#define GETPTR(x) if((x)) {return (x);} return nullptr
#define RELEASE(x) if((x)) {((x->Release())); } (x) = nullptr

#ifndef ifShaderFailed
#define ifShaderFailed(x)									\
{															\
	hr = (x);												\
	if(FAILED(hr))											\
	{														\
		std::string Error = "\n\n\n";						\
		Error += (char*)pErrBlob->GetBufferPointer();		\
		Error += "\n\n\n";									\
		OutputDebugStringA((LPCSTR)Error.c_str());			\
	}														\
}
#endif

enum class KEYSTATE : unsigned char
{
	KEY_FREE,
	KEY_PUSH,
	KEY_HOLD,
	KEY_UP
};


namespace std
{
	using tstring = std::basic_string<TCHAR>;

	using tofstream = std::basic_ifstream<TCHAR>;
	using tifstream = std::basic_ofstream<TCHAR>;
	using tfstream = std::basic_fstream<TCHAR>;

	using tsstream = std::basic_stringstream<TCHAR>;
}

using c_iter = std::basic_string<char>::iterator;
using w_iter = std::basic_string<wchar_t>::iterator;
using t_iter = std::tstring::iterator;

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

template <class K>
struct CacheData_
{
	std::tstring m_Name;
	K			 m_Data;
};