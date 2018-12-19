#pragma once

#include "ZXCWriter.h"

class ZXCExporter
{
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

	void LoadObject(const std::vector<INode*> objects);
	void LoadAnimation(INode * node, ZXCObject& o);
	void LoadMesh(INode* node, ZXCObject& o);

	void AddObject(INode* node);
	void AddMaterial(INode* node);

private:
	template<typename X>
	void ConvertVector(const X& src, X& dest)
	{
		X copysrc = src; 
		copysrc.y = src.z;
		copysrc.z = src.y;
		CopyMemory(&dest, &copysrc, sizeof(X));
	}


	void ConvertMatrix(const Matrix3& maxMatrix, D3D_MATRIX& dxMatrix);
	TriObject* GetTriObject(Object* obj, TimeValue t ,bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

	bool isTMNegative(const Matrix3& m);

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

	std::vector<ZXCObject> mOutputGeomObjects;
	std::vector<ZXCObject> mOutputHelperObjects;

	std::wstring mVersion;
	std::wstring mFilename;

	std::unique_ptr<ZXCWriter> mWriter = nullptr;
};