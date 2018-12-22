#pragma once

#include "AnimationExporter.h"

class ZXCExporter;

class ObjectExporter
{
public:
	ObjectExporter(ZXCExporter * exporter);

public:
	void LoadObject(INode ** nodearray, size_t arraysize, 
		std::vector<std::unique_ptr<ZXCObject>>& objectlist,
		std::unordered_map<std::wstring, size_t>& nodeIndex);

private:
	void LoadMesh(INode* node, ZXCObject* o);
	void BuildVBIB(ZXCObject* mesh);

	TriObject* GetTriObject(Object* obj, TimeValue t, bool & isDelete);
	Point3 GetVertexNormal(Mesh& mesh, int faceNo, const RVertex& rv);
	int GetMaterialRef(Mtl* mtl);

private:
	ZXCExporter* mExporter = nullptr;
};