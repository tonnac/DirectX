#pragma once

#include "ZXCWriter.h"

class ZXCExporter
{
public:
	ZXCExporter();
	~ZXCExporter() = default;

public:
	bool Initialize(Interface* maxinterface, const MCHAR* Filename ,const MCHAR* Longdesc);
	void Run();

private:
	void LoadNode(INode* node);
	void AddObject(INode* node);
	void AddMaterial(INode* node);

	void LoadMaterial();
	void LoadTexture(ZXCMaterial& zxcMtl, Mtl* srcMtl);

private:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::vector<Mtl*> mMaxMaterial;
	std::vector<ZXCMaterial> mOutputMaterial;

	std::wstring mVersion;
	std::wstring mFilename;

	ZXCWriter mWriter;
};