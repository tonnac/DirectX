#pragma once

#include "AnimationExporter.h"

class ZXCExporter;

class ObjectExporter
{
public:
	ObjectExporter(ZXCExporter * exporter);

public:
	ZXCMap LoadObject(const std::vector<INode*> objects);

protected:
	virtual std::unique_ptr<ZXCObject> MakeObject();
	virtual void LoadMesh(INode* node, ZXCObject* o);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

protected:
	ZXCExporter* mExporter = nullptr;
	void * biped = nullptr;
};