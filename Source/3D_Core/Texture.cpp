#include "Texture.h"

bool Texture::Release()
{
	RELEASE(m_pTexSRV);
	RELEASE(m_pSamplerState);
	return true;
}
HRESULT Texture::LoadTextureFile(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& szFileName)
{
	HRESULT hr;
	this->Name = Name;
	V_RETURN(D3DX11CreateShaderResourceViewFromFile(pDevice, szFileName.c_str(), nullptr, nullptr, &m_pTexSRV, nullptr));

	D3D11_SAMPLER_DESC samplerdesc;
	ZeroMemory(&samplerdesc, sizeof(D3D11_SAMPLER_DESC));
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	V_RETURN(pDevice->CreateSamplerState(&samplerdesc, &m_pSamplerState));

	return hr;
}
ID3D11ShaderResourceView* Texture::getTexSRV()
{
	GETPTR(m_pTexSRV);
}
ID3D11SamplerState* Texture::getSamplerState()
{
	GETPTR(m_pSamplerState);
}
const std::tstring& Texture::getName()
{
	return Name;
}
bool TextureMgr::Release()
{
	for (auto it : m_TextureList)
	{
		it.second->Release();
		delete it.second;
	}
	m_TextureList.clear();
	return true;
}
const std::tstring& TextureMgr::LoadTexture(ID3D11Device * pDevice, const std::tstring& Name, const std::tstring& Filepath)
{
	if (getTexturePtr(Name))
	{
		return Name;
	}

	TextureIter it;
	it = m_TextureList.find(Name);
	if (it != m_TextureList.end())
	{
		AddCache(it->first, it->second);
		return Name;
	}

	Texture * newData = new Texture;
	if (FAILED(newData->LoadTextureFile(pDevice,Name ,Filepath))) return nullptr;

	m_TextureList.insert(std::make_pair(Name, newData));
	AddCache(Name, newData);
	return Name;
}

void TextureMgr::AddCache(const std::tstring& Name, Texture* pShader)
{
	CacheData_<Texture*> Cache_;
	Cache_.m_Name = Name;
	Cache_.m_Data = pShader;
	if (m_CacheList.size() >= 3)
	{
		m_CacheList.pop_front();
	}
	m_CacheList.push_back(Cache_);
}
Texture* TextureMgr::getTexturePtr(const std::tstring& Name)
{
	CacheIter it;
	auto foo = [&Name](CacheData_<Texture*> pTex) {if (Name == pTex.m_Name) { return true; } return false;};
	it = std::find_if(m_CacheList.begin(), m_CacheList.end(), foo);
	if (it != m_CacheList.end())
	{
		return it->m_Data;
	}
	if (m_TextureList.find(Name) != m_TextureList.end())
	{
		return m_TextureList[Name];
	}
	return nullptr;
}
