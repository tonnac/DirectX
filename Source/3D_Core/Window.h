#pragma once
#include "Device.h"

class Window : public Device
{
public:
	Window();
public:
	bool			 InitWindow(HINSTANCE hInstance, UINT width , UINT height 
						,int nCmdShow, const TCHAR* strWindowTitle);
	void			 CenterWindow();
public:				 
	virtual bool	 Init();
	virtual bool	 Frame();
	virtual bool	 Render();
	virtual bool	 Release();
	virtual bool	 Run();
	virtual LRESULT  WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
protected:
	HINSTANCE		 m_hInstance;
	HWND	 		 m_hWnd;
	RECT	 		 m_rtClient;
	RECT	 		 m_rtWindow;
	UINT	 		 m_iWindowWidth;
	UINT	 		 m_iWindowHeight;
	DWORD			 m_dwStyle;
};