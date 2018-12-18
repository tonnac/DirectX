#pragma once

#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include <max.h>
#include <stdmat.h>
#include <decomp.h>
#include <CS\\bipexp.h>
#include <CS\\phyexp.h>
#include <iskin.h>

#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <fstream>
#include <memory>


struct SceneInfo
{
	int FirstFrame = -1;
	int LastFrame = -1;
	int FrameSpeed = -1;
	int TickperFrame = -1;
};

struct ZXCTexmap
{
	int SubNo = -1;

	std::wstring Filename;
};

struct ZXCMaterial
{
	std::wstring Name;
	std::wstring ClassName;

	std::vector<ZXCTexmap>	 TexMap;
	std::vector<ZXCMaterial> SubMaterial;
};

namespace std
{
	static void ReplaceString(wstring& src, const wstring& findString = L" ", const wstring replaceString = L"")
	{
		size_t pos = 0;
		while ((pos = src.find(findString)) != wstring::npos)
			src.replace(pos, findString.length(), replaceString);
	}
}