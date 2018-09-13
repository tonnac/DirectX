#pragma once
#include "Define.h"

class Shader
{
public:
	Shader() = default;
public:
	bool								Release();
	HRESULT								CreateShader(ID3D11Device* pDevice, const std::tstring& Filename, const std::string& vs, const std::string& ps);
//	HRESULT								LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName);
	ID3D11VertexShader *				getVertexShader();
	ID3D11PixelShader*					getPixelShader();
	ID3DBlob *							getBlob();
	const std::tstring&					getName() const;
private:
	ID3D11VertexShader *				m_pVertexShader = nullptr;
	ID3D11PixelShader*					m_pPixelShader = nullptr;
private:
	ID3DBlob*							m_pVSBlob = nullptr;
	std::tstring						m_Name;
	DWORD								m_dwFlag = D3DCOMPILE_DEBUG;
};

class ShaderMgr : public Singleton<ShaderMgr>
{
	using SharedIter = std::map<std::tstring, Shader*>::iterator;
	using CacheIter = std::list<CacheData_<Shader*>>::iterator;
	friend class Singleton<ShaderMgr>;
public:
	ShaderMgr() = default;
	~ShaderMgr() = default;
public:
	bool								Release();
public:
	const std::tstring&					LoadShader(ID3D11Device * pDevice, const std::tstring& Name, const std::tstring& Filepath, const std::string& vs = "VS", const std::string& ps = "PS");
	Shader*								getShaderPtr(const std::tstring& Name);
private:
	void								AddCache(const std::tstring& Name, Shader* pShader);
private:
	std::map<std::tstring, Shader*>		m_ShaderList;
	std::list<CacheData_<Shader*>>		m_CacheList;
};

#define S_Shader ShaderMgr::getInstance()