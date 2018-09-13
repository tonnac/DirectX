#pragma once
#include "Texture.h"
#include <list>

class ShaderMgr : public Singleton<ShaderMgr>
{
	friend class Singleton<ShaderMgr>;
	struct CacheData_
	{
		std::tstring	m_tName;
		Shader* m_bBitmap;
	};
public:
	ShaderMgr() = default;
	~ShaderMgr() = default;
public:
	bool						Init();
	bool						Frame();
	bool						Render();
	bool						Release();
public:
	std::tstring				Load(ID3D11Device * pDevice, std::tstring, std::tstring);
	Shader*						getBitmapPtr(std::tstring);
private:
	void						AddCache(std::tstring, Texture*);
private:
	std::map<std::tstring, Texture*>	m_BitmapList;
	std::list<CacheData_>		m_CacheList;
};

#define S_ShaderMgr	TextureMgr::getInstance()