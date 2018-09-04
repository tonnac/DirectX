#include "Window.h"

Window * g_pWindow = nullptr;
HWND	 g_hWnd = nullptr;
RECT	 g_rtClient = { 0, };
BOOL	 g_bActiveApp = FALSE;

LRESULT CALLBACK StaticWndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	assert(g_pWindow != nullptr);
	return g_pWindow->WndProc(hwnd, msg, wparam, lparam);
}

Window::Window()
{
	g_pWindow = this;
	m_hWnd = nullptr;
	m_hInstance = nullptr;
}

bool Window::InitWindow(HINSTANCE hInstance, UINT width, UINT height
	, int nCmdShow, const TCHAR* strWindowTitle)
{
	WNDCLASSEX winclass;
	ZeroMemory(&winclass, sizeof(WNDCLASSEX));
	winclass.cbSize = sizeof(WNDCLASSEX);
	winclass.style = CS_HREDRAW | CS_VREDRAW;
	winclass.lpfnWndProc = StaticWndProc;
	winclass.hInstance = m_hInstance = hInstance;
	winclass.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	winclass.lpszMenuName = strWindowTitle;
	winclass.lpszClassName = L"Window";
	winclass.hIcon = LoadIcon(nullptr, IDI_SHIELD);
	winclass.hCursor = LoadCursor(nullptr, IDC_ARROW);
	winclass.hIconSm = LoadIcon(nullptr, IDI_SHIELD);

	if (RegisterClassEx(&winclass) == false)
	{
		return false;
	}

	m_dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	RECT rt = { 0, 0, 
		static_cast<LONG>(width),
		static_cast<LONG>(height) };
	AdjustWindowRect(&rt, WS_OVERLAPPEDWINDOW, FALSE);

	m_hWnd = g_hWnd = CreateWindowEx(WS_EX_APPWINDOW,
		L"Window", 
		strWindowTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT ,CW_USEDEFAULT,
		rt.right - rt.left, 
		rt.bottom - rt.top,
		NULL,NULL,
		m_hInstance,
		NULL);

	assert(m_hWnd != nullptr);

	m_dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);

	GetWindowRect(m_hWnd, &m_rtWindow);
	GetClientRect(m_hWnd, &m_rtClient);

	g_rtClient = m_rtClient;

	m_iWindowWidth = m_rtClient.right - m_rtClient.left;
	m_iWindowHeight = m_rtClient.bottom - m_rtClient.top;

	CenterWindow();
	
	ShowWindow(m_hWnd, SW_SHOW); // 풀스크린 쓸때는 안써도된다.
	return true;
}
void Window::CenterWindow()
{
	int iScreenWidth = GetSystemMetrics(SM_CXFULLSCREEN);
	int iScreenHeight = GetSystemMetrics(SM_CYFULLSCREEN);

	int x = (iScreenWidth - (m_rtClient.right - m_rtClient.left)) / 2;
	int y = (iScreenHeight - (m_rtClient.bottom - m_rtClient.top)) / 2;

	MoveWindow(m_hWnd, x, y, m_rtClient.right - m_rtClient.left, m_rtClient.bottom - m_rtClient.top, TRUE);
}
bool Window::GameInit()
{
	return true;
}
bool Window::GameRun()
{
	return true;
}
bool Window::GameRelease()
{
	return true;
}
bool Window::Run()
{
	if (GameInit() == false) return false;
	MSG msg;
	ZeroMemory(&msg, sizeof(MSG));

	while (msg.message != WM_QUIT)
	{
		if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else
		{
			GameRun();
		}
	}
	if (GameRelease() == false) return false;
	return true;
}
LRESULT Window::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	IDXGISwapChain* pSwapChain = getSwapChain();
	BOOL isFullMode = FALSE;
	UINT width = LOWORD(lparam);
	UINT height = HIWORD(lparam);
	switch (msg)
	{
	case WM_SIZE:
		ResizeDevice(width, height);
		break;
	case WM_KEYDOWN:
		switch (wparam)
		{
		case '0':
			pSwapChain->GetFullscreenState(&isFullMode, NULL);
			pSwapChain->SetFullscreenState(!isFullMode, NULL);
			if (isFullMode == TRUE)
			{
				ShowWindow(m_hWnd, SW_SHOW);
			}
			break;
		case '1':
			if (pSwapChain)
			{
				DXGI_MODE_DESC desc;
				ZeroMemory(&desc, sizeof(DXGI_MODE_DESC));
				desc.Width = 1024;
				desc.Height = 768;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				desc.RefreshRate.Numerator = 60;
				desc.RefreshRate.Denominator = 1;
				pSwapChain->ResizeTarget(&desc);
			}
			break;
		case '2':
			if (pSwapChain)
			{
				DXGI_MODE_DESC desc;
				ZeroMemory(&desc, sizeof(DXGI_MODE_DESC));
				desc.Width = 800;
				desc.Height = 600;
				desc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
				desc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
				desc.RefreshRate.Numerator = 60;
				desc.RefreshRate.Denominator = 1;
				pSwapChain->ResizeTarget(&desc);
			}
			break;
		}
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_ACTIVATEAPP:
		g_bActiveApp = static_cast<BOOL>(wparam);
		break;
	}
	return DefWindowProc(hwnd, msg, wparam, lparam);
}