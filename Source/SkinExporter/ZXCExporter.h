#pragma once

#include "ObjectExporter.h"
#include "ZXCWriter.h"

class ZXCExporter
{
	friend class ObjectExporter;
public:
	ZXCExporter();
	~ZXCExporter() = default;

public:
	bool Initialize(Interface* maxinterface, const MCHAR* Filename ,const MCHAR* Longdesc);
	bool Run();

private:
	void LoadScene();
	void LoadNode(INode* node);
	void LoadMaterial();
	void LoadTexture(ZXCMaterial& zxcMtl, Mtl* srcMtl);

	void AddObject(INode* node);
	void AddMaterial(INode* node);

private:
	bool CheckForAnimation(INode* node, bool& isPos, bool& isRot, bool& isScale);
	void DecompAffine(TimeValue t, INode * node, AffineParts& ap, Point3* rotAxis = nullptr, float* rotAngle = nullptr);

	void InputPosSample(INode* node, ZXCObject& o);
	void InputRotSample(INode* node, ZXCObject& o);
	void InputSclaeSample(INode* node, ZXCObject& o);

	void OverlappedTrackErase(ZXCObject& o);

private:
	Interface*	mMaxInterface = nullptr;
	INode*		mRootNode = nullptr;
	Interval	mInterval;

	SceneInfo	mSceneInfo;

	std::vector<Mtl*> mMaxMaterial;
	std::vector<ZXCMaterial> mOutputMaterial;

	std::vector<INode*> mHelperObjects;
	std::vector<INode*> mGeomObjects;

	std::wstring mVersion;
	std::wstring mFilename;

	std::unique_ptr<ZXCWriter> mWriter = nullptr;
};