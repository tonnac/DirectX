#pragma once

#include "Header.h"

using WeightMap = std::map<float, std::wstring, std::greater<float>>;
using WeightIter = WeightMap::const_iterator;

struct BipedVertex
{
	std::wstring mNodename;
	int			 mNumWeight;
	WeightMap	 mWeightList;
};

struct PNCTW4VERTEX
{
	PNCT_VERTEX v[3];
	std::array<float, 4> w;
	std::array<float, 4> i;
};

struct PNCTW8VERTEX
{
	PNCT_VERTEX v[3];
	std::array<float, 4> w0;
	std::array<float, 4> i0;
	std::array<float, 4> w1;
	std::array<float, 4> i1;
};

struct SkinTri
{
	int mSubMtrl = -1;
#if MAXWEIGHTNUM == 8
	std::array<PNCTW8VERTEX, 3> v;
#else
	std::array<PNCTW4VERTEX, 3> v;
#endif
};

struct SkinMesh : public ZXCObject
{
	std::vector<SkinTri> mSkinTriList;
	std::vector<std::unique_ptr<SkinMesh>> mSubSkinMesh;
};

using VectorBiped = std::vector<BipedVertex>;

class SkinExporter : public Singleton<SkinExporter>
{
	friend class Singleton<SkinExporter>;
private:
	SkinExporter() = default;

public:
	bool isBipedObject(INode* node);
	void LoadBipedInfo(INode* node, VectorBiped* bipedes, TimeValue t);

private:

	Modifier* FindModifer(INode * node, Class_ID classID);

	void ExportPhysiqueData(INode* node, VectorBiped* bipedes, Modifier* phyMod, TimeValue t);
	void ExportSkinData(INode* node, VectorBiped* bipedes, Modifier* skinMod);

};

#define S_SkinEx SkinExporter::getInstance()