#pragma once
#include "Object.h"

class Shader
{
public:
	Shader() = default;
public:
	bool				Init();
	bool				Frame();
	bool				Render();
	bool				Release();
	HRESULT				CreateShader(ID3D11Device* pDevice, const std::tstring& Filename, const std::string& vs = "VS", const std::string& ps = "PS");
//	HRESULT				LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName);
private:
	ID3DBlob*					m_pVSBlob = nullptr;
	ID3D11ShaderResourceView*	m_pTexSRV = nullptr;
	std::tstring				m_szName;
};
