#pragma once
#include "Texture.h"
#include <list>



class TextureMgr : public Singleton<TextureMgr>
{
	struct CacheData_
	{
		std::tstring	m_tName;
		Texture* m_bBitmap;
	};
	friend class Singleton<TextureMgr>;
public:
	TextureMgr() = default;
	~TextureMgr() = default;
public:
	bool						Init();
	bool						Frame();
	bool						Render();
	bool						Release();
public:
	std::tstring				Load(ID3D11Device * pDevice,std::tstring, std::tstring);
	Texture*					getBitmapPtr(std::tstring);
private:
	void						AddCache(std::tstring, Texture*);
private:
	std::map<std::tstring, Texture*>	m_BitmapList;
	std::list<CacheData_>		m_CacheList;
};

#define S_TextureMgr	TextureMgr::getInstance()