#include "Sample.h"
#include "DirectWrite.h"
#include <DirectXMath.h>


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_hImc = ImmGetContext(g_hWnd);
	ImmNotifyIME(m_hImc, NI_COMPOSITIONSTR, CPS_CANCEL, 0); // 조합중 취소.
//	m_hImc = ImmCreateContext();
	return true;
}
bool Sample::Frame()
{
	if (S_Input.getKeyState(DIK_NUMPADMINUS) == KEYSTATE::KEY_PUSH)
	{
		m_stringList.clear();
	}
	return true;
}
bool Sample::Render()
{
	S_DirectWrite.DrawText({ 0.0f, 50.0f, 800.0f, 600.0f }, m_CompWord.c_str(), D2D1::ColorF::White);
	S_DirectWrite.DrawText({ 0.0f, 75.0f, 800.0f, 600.0f }, m_Compstring.c_str(), D2D1::ColorF::White);
	S_DirectWrite.DrawText({ 0.0f, 125.0f, 800.0f, 600.0f }, m_Conversion.c_str(), D2D1::ColorF::White);

	m_CurrentString.clear();
	m_CurrentString = m_Compstring + m_CompWord;
	S_DirectWrite.DrawText({ 0.0f, 100.0f, 800.0f, 600.0f }, m_CurrentString.c_str(), D2D1::ColorF::White);

	for (int i = 0; i < m_stringList.size(); ++i)
	{
		S_DirectWrite.DrawText({ 0.0f, 125.0f + (i + 1)*25.0f, 800.0f, 600.0f }, m_stringList[i].c_str(), D2D1::ColorF::White);
	}

	return true;
}
bool Sample::Release()
{
	ImmReleaseContext(g_hWnd, m_hImc);
	return true;
}
LRESULT	Sample::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	switch (msg)
	{
		case WM_IME_NOTIFY:
		{
			if (wparam == IMN_SETCONVERSIONMODE)
			{
				DWORD dwConversion;
				DWORD dwSentence;
				ImmGetConversionStatus(m_hImc, &dwConversion, &dwSentence);
				// 한글
				if (m_dwConversion != dwConversion)
				{
					if (dwConversion & IME_CMODE_NATIVE)
					{
						m_Conversion = L"한글";
					}
					else // 영문
					{
						m_Conversion = L"영어";
					}
					m_dwConversion = dwConversion;
				}
			}
		}break;
		case WM_IME_STARTCOMPOSITION:
		{
			m_CompWord.clear();
			return 1;
		}
		case WM_IME_ENDCOMPOSITION:
		{
			m_CompWord = wparam;
			return 1;
		}
		case WM_IME_COMPOSITION:
		{
			if (lparam & GCS_RESULTSTR)
			{
				TCHAR szBackup[256] = { 0, };
				m_nComSize = ImmGetCompositionStringW(m_hImc, GCS_RESULTSTR, (LPVOID)szBackup, sizeof(TCHAR) * 256);
				if (m_nComSize > 0)
				{
					m_Compstring += szBackup;
				}
			}
			else if (lparam & GCS_COMPSTR)
			{
				TCHAR szBackup[256] = { 0, };
				m_nComSize = ImmGetCompositionStringW(m_hImc, GCS_COMPSTR, (LPVOID)szBackup, sizeof(TCHAR) * 256);
				m_CompWord = szBackup;
			}
			else
			{
				m_nComSize = 0;
			}
			ImmReleaseContext(g_hWnd, m_hImc);
			return 1;
		}
		case WM_IME_CHAR:
		{
			m_Compstring += wparam;
			return 1;
		}
		case WM_CHAR:
		{
			switch (wparam)
			{
				case VK_BACK:
				{
					if (m_Compstring.empty() == false)
					{
						t_iter it = m_Compstring.begin() + (m_Compstring.size() - 1);
						m_Compstring.erase(it);
					}
				}break;
				case VK_RETURN:
				{
					m_stringList.push_back(m_CurrentString);
					ResetCompositionString();
				}break;
				default:
				{
					m_Compstring += wparam;
				}break;
			}
		}
		default:
		{

		}break;
	}
	return Window::WndProc(hwnd, msg, wparam, lparam);
}
void Sample::ResetCompositionString()
{
	m_Compstring.clear();
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"Input Method Editer");
	wd.Run();
	return 0;
}