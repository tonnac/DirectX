#pragma once

#include "Header.h"
#include <chrono>

class ZXCWriter
{
public:
	bool Savefile(
		const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCMaterial>& material);

private:
	void InputMaterial(const std::vector<ZXCMaterial>& material, std::wofstream& os);

	std::wstring Savetime();

};