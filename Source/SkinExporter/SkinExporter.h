#pragma once

#include "ObjectExporter.h"

using WeightMap = std::map<float, std::wstring, std::greater<float>>;
using WeightIter = WeightMap::const_iterator;

struct BipedVertex
{
	std::wstring mNodename;
	int			 mNumWeight;
	WeightMap	 mWeightList;
};

struct SkinTri
{
	int mSubMtrl = -1;
#if MAXWEIGHTNUM == 8
	std::array<PNCTW8VERTEX, 3> v;
#else
	std::array<PNCTW4VERTEX, 3> v;
#endif
	inline bool operator< (const SkinTri& rhs) const
	{
		return mSubMtrl < rhs.mSubMtrl;
	}
};

struct SkinMesh : public ZXCObject
{
	std::vector<SkinTri> mSkinTriList;
	std::vector<std::unique_ptr<SkinMesh>> mSubSkinMesh;
};

using VectorBiped = std::vector<BipedVertex>;

class SkinExporter : public ObjectExporter
{
public:
	SkinExporter(ZXCExporter* exporter);
private:
	void LoadBipedInfo(INode* node, VectorBiped* bipedes);
	virtual void LoadMesh(INode* node, ZXCObject* o)override;
	virtual std::unique_ptr<ZXCObject> MakeObject()override;

private:
	void InputBipedes(PNCTW4VERTEX& vertex, const BipedVertex& bipedes);
	void ExportPhysiqueData(INode* node, VectorBiped* bipedes, Modifier* phyMod);
	void ExportSkinData(INode* node, VectorBiped* bipedes, Modifier* skinMod);

};