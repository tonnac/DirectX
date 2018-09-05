#include "Sample.h"
#include <cmath>

Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_DirectWrite.Init();
	IDXGISurface1* pSurface = nullptr;
	IDXGISwapChain* pSwapChain = getSwapChain();

	pSwapChain->GetBuffer(0, __uuidof(IDXGISurface), (LPVOID*)&pSurface);
	m_DirectWrite.Set(g_rtClient.right, g_rtClient.bottom, pSurface);

	pSurface->Release();

	TextArray s0, s1, s2;

	s0.m_Text = L"aaaa";
	s0.m_Text = L"bbb";
	s0.m_Text = L"cccc";

//	m_DirectWrite.push();
	return true;
}
bool Sample::Render()
{
	IDWriteTextFormat* pFormat = m_DirectWrite.getTextFormat();
	pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);				// 가로 정렬
	pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);		// 세로 정렬
	D2D1_RECT_F rf = D2D1::RectF(g_rtClient.left, g_rtClient.top, g_rtClient.right, g_rtClient.bottom);
	m_DirectWrite.DrawText(rf, L"KGCA", D2D1::ColorF(1.0f, 1.0f, 0, cosf(g_fGameTimer)*0.5 + 0.5));	// 알파값으로 투명도를 조절
//	m_DirectWrite.Render();
	return true;
}
bool Sample::Release()
{
	m_DirectWrite.Release();
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleDirectWrite");
	wd.Run();
	return 0;
}