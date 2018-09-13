#pragma once
#include "Define.h"

class Texture
{
public:
	bool								Release();
	HRESULT								LoadTextureFile(ID3D11Device* pDevice,const std::tstring& Name ,const std::tstring& szFileName);
	ID3D11ShaderResourceView*			getTexSRV();
	ID3D11SamplerState*					getSamplerState();
	const std::tstring&					getName();
private:
	ID3D11ShaderResourceView *			m_pTexSRV;
	ID3D11SamplerState*					m_pSamplerState;
	std::tstring						Name;
};

class TextureMgr : public Singleton<TextureMgr>
{
	using TextureIter = std::map<std::tstring, Texture*>::iterator;
	using CacheIter = std::list<CacheData_<Texture*>>::iterator;
	friend class Singleton<TextureMgr>;
public:
	TextureMgr() = default;
	~TextureMgr() = default;
public:
	bool								Release();
public:
	const std::tstring&					LoadTexture(ID3D11Device * pDevice, const std::tstring& Name, const std::tstring& szFile);
	Texture*							getTexturePtr(const std::tstring& szName);
private:
	void								AddCache(const std::tstring& szName, Texture* pShader);
private:
	std::map<std::tstring, Texture*>	m_TextureList;
	std::list<CacheData_<Texture*>>		m_CacheList;
};

#define S_TextureMgr	TextureMgr::getInstance()