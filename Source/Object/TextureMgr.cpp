#include "TextureMgr.h"

bool TextureMgr::Init()
{
	return true;
}
bool TextureMgr::Frame()
{
	return true;
}
bool TextureMgr::Render()
{
	return true;
}
bool TextureMgr::Release()
{
	for (auto it : m_BitmapList)
	{
		it.second->Release();
		delete it.second;
	}
	m_BitmapList.clear();
	return true;
}
std::tstring TextureMgr::Load(ID3D11Device * pDevice, std::tstring szName, std::tstring szFile)
{
	if (getBitmapPtr(szName))
	{
		return szName;
	}

	std::map<T_STR, Bitmap*>::iterator it;
	it = m_BitmapList.find(szName);
	if (it != m_BitmapList.end())
	{
		AddCache(it->first, it->second);
		return szName;
	}

	Bitmap * newBitmap = New Bitmap;
	if (!newBitmap->LoadFile(szFile)) return nullptr;

	m_BitmapList.insert(std::make_pair(szName, newBitmap));
	AddCache(szName, newBitmap);
	return szName;
}

void TextureMgr::AddCache(std::tstring szName, Bitmap* hBItmap)
{
	CacheData_ Cache_;
	Cache_.m_tName = szName;
	Cache_.m_bBitmap = hBItmap;
	if (m_CacheList.size() >= 3)
	{
		m_CacheList.pop_front();
	}
	m_CacheList.push_back(Cache_);
}
Bitmap* TextureMgr::getBitmapPtr(T_STR szName)
{
	for (auto it : m_CacheList)
	{
		if (it.m_tName == szName)
			return it.m_bBitmap;
	}
	for (auto it2 : m_BitmapList)
	{
		if (it2.first == szName)
			return it2.second;
	}
	return nullptr;
}
