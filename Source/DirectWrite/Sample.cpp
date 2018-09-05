#include "Sample.h"

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
	IDXGISwapChain* pSwapChain = getSwapChain();
	ID3D11DeviceContext* pContext = getContext();
	ID3D11RenderTargetView* pRenderTarget = getRenderTargetView();
	float ClearColor[4] = { 0.0f, 0.125f, 0.3f, 1.0f }; //red,green,blue,alpha
	pContext->ClearRenderTargetView(pRenderTarget, ClearColor);
	IDWriteTextFormat* pFormat = m_DirectWrite.getTextFormat();
	pFormat->SetTextAlignment(DWRITE_TEXT_ALIGNMENT_CENTER);				// 가로 정렬
	pFormat->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_CENTER);		// 세로 정렬

	m_DirectWrite.DrawText(g_rtClient, L"KGCA", D2D1::ColorF(1.0f, 1.0f, 0, 1.0f));	// 알파값으로 투명도를 조절
	pContext->OMSetRenderTargets(1, &pRenderTarget, NULL);
	pSwapChain->Present(0, 0);


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