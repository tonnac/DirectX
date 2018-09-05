#pragma once
#include "Define.h"

#include <d2d1.h>
#include <dwrite.h>
#pragma comment(lib, "d2d1.lib")
#pragma comment(lib, "dwrite.lib")

struct TextArray
{
	TextArray()
	{
		matWorld = D2D1::IdentityMatrix();
		m_Color = D2D1::ColorF(0,0,0,1);
		m_rt.left = (FLOAT)g_rtClient.left;
		m_rt.top = (FLOAT)g_rtClient.top;
		m_rt.right = (FLOAT)g_rtClient.right;
		m_rt.bottom = (FLOAT)g_rtClient.bottom;
	}
	D2D1_RECT_F			m_rt;
	D2D1_MATRIX_3X2_F	matWorld;
	D3DCOLORVALUE		m_Color;
	std::wstring		m_Text;
	// IDWriteTextFormat*  m_pTextFormat;
};

class DirectWrite
{
public:
	DirectWrite();
public:
	bool					Set								(const int& width, const int& height, IDXGISurface* pSurface);
	HRESULT					CreateDeviceIndependentResources();
	HRESULT					CreateDeviceResources			(IDXGISurface* pSurface);
	void					DiscardDeviceResources			();
	HRESULT					DrawText						(const RECT& rt, const TCHAR* pText, D2D1::ColorF color);
	void					OnResize						(const int& width, const int& height, IDXGISurface* pSurface);
	bool					Init							();
	bool					Frame							();
	bool					Render							();
	bool					Release							();
public:
	IDWriteTextFormat * getTextFormat();
private:
	ID2D1Factory *			m_pd2dFactory;
	IDWriteFactory*			m_pWriteFactory;
	ID2D1RenderTarget*		m_pRenderTarget;
	IDWriteTextFormat *		m_pTextFormat;
	ID2D1SolidColorBrush*	m_pColorBrush;
	FLOAT					m_fDpiX;
	FLOAT					m_fDpiY;
	std::vector<TextArray>	m_TextList;
};