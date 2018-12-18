
#include "ZXCWriter.h"
#include <codecvt>

using namespace std::chrono;

bool ZXCWriter::Savefile(
	const std::wstring & ExporterVersion,
	const std::wstring & Filename,
	const SceneInfo & sceneinfo,
	const std::vector<ZXCMaterial>& material)
{
	std::wofstream oss;
	oss.open(Filename.c_str());

	if (!oss.is_open()) return false;

	std::wstring nowTime = Savetime();

	oss << ExporterVersion << std::endl << nowTime << std::endl;

	std::wstring sceneStart = L"#SCENEINFO";
	std::wstring firstFrame = L"\n\tFirstFrmae: " + std::to_wstring(sceneinfo.FirstFrame);
	std::wstring lastFrame = L"\n\tLastFrame: " + std::to_wstring(sceneinfo.LastFrame);
	std::wstring frameSpeed = L"\n\tFrameSpeed: " + std::to_wstring(sceneinfo.FrameSpeed);
	std::wstring tickPerFrame = L"\n\ttickPerFrame: " + std::to_wstring(sceneinfo.TickperFrame) + L"\n";

	oss << sceneStart << firstFrame << lastFrame << frameSpeed << tickPerFrame;

	InputMaterial(material, oss);

	return true;
}

void ZXCWriter::InputMaterial(const std::vector<ZXCMaterial>& material, std::wofstream & os)
{
	std::wstring rootMtlInfo = L"\n#MATERIAL_NUM " + std::to_wstring(material.size());

	os << rootMtlInfo;

	for (int i = 0; i < (int)material.size(); ++i)
	{
		std::wstring mtlInfo = L"\n\t#MATERIAL_INDEX " + std::to_wstring(i) +
			L"\t#MATERIAL_NAME " + material[i].Name +
			L"\t#MAP_NUM " + std::to_wstring(material[i].TexMap.size()) + 
			L"\t#SUBMTL_NUM " + std::to_wstring(material[i].SubMaterial.size());

		os << mtlInfo;

		if (!material[i].SubMaterial.empty())
		{
			for (int k = 0; k < material[i].SubMaterial.size(); ++k)
			{
				std::wstring subMtlInfo = L"\n\t\t#SUBMTL_INDEX " + std::to_wstring(k) +
					L"\t#MATERIAL_NAME " + material[i].SubMaterial[k].Name +
					L"\t#MAP_NUM " + std::to_wstring(material[i].SubMaterial[k].TexMap.size());

				os << subMtlInfo;

				if (!material[i].SubMaterial[k].TexMap.empty())
				{
					for (auto &x : material[i].SubMaterial[k].TexMap)
					{
						std::wstring TexInfo = L"\n\t\t\t#MAP_SUBNO " + std::to_wstring(x.SubNo) +
							L"\t#TEX_FILE " + x.Filename;
						os << TexInfo;
					}
				}
			}
			os << std::endl;
		}
		else
		{
			if (!material[i].TexMap.empty())
			{
				for (auto &x : material[i].TexMap)
				{
					std::wstring TexInfo = L"\n\t\t#MAP_SUBNO " + std::to_wstring(x.SubNo) +
						L"\t#TEX_FILE " + x.Filename;
					os << TexInfo;
				}
			}
			os << std::endl;
		}
	}
	os << std::endl;
}

std::wstring ZXCWriter::Savetime()
{
	system_clock::time_point now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);

	char buf[256] = { 0, };
	wchar_t wbuf[256] = { 0, };
	ctime_s(buf, sizeof(buf), &nowTime);

	MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 256);

	return std::wstring(wbuf);
}
