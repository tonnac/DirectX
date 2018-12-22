#pragma once

#include "Header.h"
#include <chrono>

struct SkinMesh;

class ZXCWriter
{
public:
	ZXCWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCMaterial>& material,
		std::vector<std::unique_ptr<ZXCObject>>& object,
		AnimationClip * clip);
public:
	bool Savefile();

private:
	void WriteScene(std::wofstream& os);
	void WriteMaterial(std::wofstream& os);
	void WriteMesh(std::wofstream& os);
	void WriteHelper(std::wofstream& os);
	void WriteVertex(std::wofstream& os);
	void WriteAnimations(std::wofstream& os);

	std::wstring Savetime();

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;

	const SceneInfo& mSceneInfo;
	const std::vector<ZXCMaterial>& mMaterial;
	std::vector<std::unique_ptr<ZXCObject>> mObjects;
	AnimationClip * mAnimation = nullptr;
};

