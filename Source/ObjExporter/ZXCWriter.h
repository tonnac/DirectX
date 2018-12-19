#pragma once

#include "Header.h"
#include <chrono>

class ZXCWriter
{
public:
	ZXCWriter(const std::wstring& ExporterVersion,
		const std::wstring& Filename,
		const SceneInfo& sceneinfo,
		const std::vector<ZXCMaterial>& material,
		std::vector<ZXCObject>& mesh,
		std::vector<ZXCObject>& helper);
public:
	bool Savefile();

private:
	void InputScene(std::wofstream& os);
	void InputMaterial(std::wofstream& os);
	void InputMesh(std::wofstream& os);
	void InputHelper(std::wofstream& os);

	void InputVBIB(ZXCObject& mesh, int mtrlRef, std::wofstream& os);
	void InputAnimation(const ZXCObject& obj, std::wofstream& os);

	std::wstring Savetime();
	std::wstring VertexToString(const PNCT_VERTEX& v);

	const std::wstring& mExporterVersion;
	const std::wstring& mFilename;
	const SceneInfo& mSceneInfo;
	const std::vector<ZXCMaterial>& mMaterial;
	std::vector<ZXCObject> mMesh;
	std::vector<ZXCObject> mHelper;
};