#pragma once

#include "AnimationExporter.h"
#include "SkinExporter.h"

class ZXCExporter;

class ObjectExporter
{
public:
	ObjectExporter(ZXCExporter * exporter);

public:
	std::unordered_map<std::wstring, ZXCObject> LoadObject(const std::vector<INode*> objects);

private:
	ZXCObject& LoadMesh(INode* node, ZXCObject& o);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

private:
	ZXCExporter* mExporter = nullptr;
};