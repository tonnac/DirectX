#pragma once
#include "Object.h"

class Texture
{
	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11SamplerState*			m_pSamplerState;
	std::tstring				Name;
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
	HRESULT				LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName);
private:
	BITMAP				m_BitmapInfo;
	HBITMAP				m_hBitmap;
	HDC					m_hMemDC;
};