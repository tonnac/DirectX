#include "DirectWrite.h"
DirectWrite::DirectWrite() : m_pd2dFactory(nullptr), m_pWriteFactory(nullptr), m_pRenderTarget(nullptr), m_pTextFormat(nullptr), m_pColorBrush(nullptr)
{}
bool DirectWrite::Set(const int& width, const int& height, IDXGISurface* pSurface)
{
	CreateDeviceIndependentResources();
	CreateDeviceResources(pSurface);
	return true;
}
HRESULT DirectWrite::CreateDeviceIndependentResources()
{
	HRESULT hr;
	if (FAILED(hr = D2D1CreateFactory(D2D1_FACTORY_TYPE_SINGLE_THREADED, &m_pd2dFactory))) //½Ì±Û ½º·¹µå
	{
		return hr;
	}
	if (FAILED(hr = DWriteCreateFactory(DWRITE_FACTORY_TYPE_SHARED, __uuidof(IDWriteFactory), (IUnknown**)&m_pWriteFactory)))
	{
		return hr;
	}

	if (FAILED(hr = m_pWriteFactory->CreateTextFormat(
		L"°íµñ",
		NULL, 
		DWRITE_FONT_WEIGHT_NORMAL,		// WEIGHT
		DWRITE_FONT_STYLE_NORMAL,		// STYLE
		DWRITE_FONT_STRETCH_NORMAL,		// STRETCH
		20,								// FontSize
		L"ko-KR",						// L"en-us"
		&m_pTextFormat)))
	{
		return hr;
	}
	return hr;
}
HRESULT DirectWrite::CreateDeviceResources(IDXGISurface* pSurface)
{
	HRESULT hr;

	D2D1_RENDER_TARGET_PROPERTIES TargetProperty;
	TargetProperty.type = D2D1_RENDER_TARGET_TYPE_DEFAULT;
	D2D1_PIXEL_FORMAT PixelFormat;
	PixelFormat.alphaMode = D2D1_ALPHA_MODE_PREMULTIPLIED;
	PixelFormat.format = DXGI_FORMAT_UNKNOWN;

	m_pd2dFactory->GetDesktopDpi(&m_fDpiX, &m_fDpiY);

	// 800 = ppi * 96.0f
	// 600 * ppi * 96.0f

	TargetProperty.pixelFormat = PixelFormat;
	TargetProperty.dpiX = m_fDpiX;
	TargetProperty.dpiY = m_fDpiY;
	TargetProperty.usage = D2D1_RENDER_TARGET_USAGE_NONE;
	TargetProperty.minLevel = D2D1_FEATURE_LEVEL_DEFAULT;
	if (FAILED(hr = m_pd2dFactory->CreateDxgiSurfaceRenderTarget(pSurface, &TargetProperty, &m_pRenderTarget)))
	{
		return hr;
	}
	if (FAILED(hr = m_pRenderTarget->CreateSolidColorBrush(D2D1::ColorF(1,1,1,0), &m_pColorBrush)))
	{
		return hr;
	}
	return hr;
}
void DirectWrite::DiscardDeviceResources()
{
	if (m_pColorBrush) m_pColorBrush->Release();
	if (m_pRenderTarget) m_pRenderTarget->Release();
	m_pRenderTarget = nullptr;
	m_pColorBrush = nullptr;
}
HRESULT DirectWrite::DrawText(const RECT& rt, const TCHAR* pText, D2D1::ColorF color)
{
	D2D1_RECT_F rf = D2D1::RectF(rt.left, rt.top, rt.right, rt.bottom);
	m_pRenderTarget->BeginDraw();
	//D2D1::Matrix3x2F matWorld;
	//D2D1_POINT_2F center;
	//center.x = 400;
	//center.y = 300;

	//FLOAT fScale = (cosf(g_fGameTimer)*0.5f + 0.5f) * 10;

	//D2D1::Matrix3x2F scale = matWorld.Rotation(g_fGameTimer, center);
	//D2D1::Matrix3x2F rot = matWorld.Scale(fScale, fScale, center);
	//rot = scale * rot;

//	m_pRenderTarget->SetTransform(D2D1::IdentityMatrix());

	m_pColorBrush->SetColor(color);
	m_pRenderTarget->DrawText(pText, static_cast<UINT>(_tcslen(pText)), m_pTextFormat, rf, m_pColorBrush);

	m_pRenderTarget->EndDraw();
	return S_OK;
}
void DirectWrite::OnResize(const int& width, const int& height, IDXGISurface* pSurface)
{
	DiscardDeviceResources();
	CreateDeviceResources(pSurface);
}
bool DirectWrite::Init()
{
	return true;
}
bool DirectWrite::Frame()
{
	return true;
}
bool DirectWrite::Render()
{
	m_pRenderTarget->BeginDraw();
	D2D1::Matrix3x2F matWorld;
	D2D1_POINT_2F center;
	center.x = 400;
	center.y = 300;

	FLOAT fScale = (cosf(g_fGameTimer)*0.5f + 0.5f) * 10;

	D2D1::Matrix3x2F scale = matWorld.Rotation(g_fGameTimer, center);
	D2D1::Matrix3x2F rot = matWorld.Scale(fScale, fScale, center);
	rot = scale * rot;

	for (int iText = 0; iText < m_TextList.size(); ++iText)
	{
		D2D1_RECT_F rf = m_TextList[iText].m_rt;
		m_pRenderTarget->SetTransform(m_TextList[iText].matWorld);
		m_pColorBrush->SetColor(m_TextList[iText].m_Color);
		m_pRenderTarget->DrawText(m_TextList[iText].m_Text.c_str(),
			m_TextList[iText].m_Text.length(),
			m_pTextFormat, rf, m_pColorBrush);
	}

	m_pRenderTarget->EndDraw();
	return true;
}
bool DirectWrite::Release()
{
	DiscardDeviceResources();
	if (m_pTextFormat) m_pTextFormat->Release();
	if (m_pWriteFactory) m_pWriteFactory->Release();
	if (m_pd2dFactory) m_pd2dFactory->Release();
	m_pTextFormat = nullptr;
	m_pWriteFactory = nullptr;
	m_pd2dFactory = nullptr;
	return true;
}
IDWriteTextFormat * DirectWrite::getTextFormat()
{
	if (m_pTextFormat)
		return m_pTextFormat;
	return nullptr;
}