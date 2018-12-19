#pragma once

#include <istdplug.h>
#include <iparamb2.h>
#include <iparamm2.h>
#include <maxtypes.h>
#include <utilapi.h>
#include <max.h>
#include <stdmat.h>
#include <decomp.h>
#include <CS\\bipexp.h>
#include <CS\\phyexp.h>
#include <iskin.h>

#include <vector>
#include <unordered_map>
#include <map>
#include <set>
#include <string>
#include <fstream>
#include <memory>
#include <algorithm>
#include <functional>

constexpr float Epsilon = 1.0e-3f;

struct D3D_MATRIX
{
	D3D_MATRIX()
	{
		m[0][0] = 1.0f, m[0][1] = 0.0f, m[0][2] = 0.0f, m[0][3] = 0.0f,
		m[1][0] = 0.0f, m[1][1] = 1.0f, m[1][2] = 0.0f, m[1][3] = 0.0f,
		m[2][0] = 0.0f, m[2][1] = 0.0f, m[2][2] = 1.0f, m[2][3] = 0.0f,
		m[3][0] = 0.0f, m[3][1] = 0.0f, m[3][2] = 0.0f, m[3][3] = 1.0f;
	};
	union {
		struct {
			float _11, _12, _13, _14;
			float _21, _22, _23, _24;
			float _31, _32, _33, _34;
			float _41, _42, _43, _44;
		};
		float m[4][4];
	};
};

struct PNCT_VERTEX
{
	Point3 p;
	Point3 n;
	Point4 c = { 1.0f, 1.0f, 1.0f, 1.0f };
	Point2 t;
	inline bool operator == (const PNCT_VERTEX& rhs)
	{
		if (p.Equals(rhs.p, Epsilon) && n.Equals(rhs.n, Epsilon) && c.Equals(rhs.c, Epsilon) && t.Equals(rhs.t, Epsilon))
		{
			return true;
		}
		return false;
	}
};

struct VertexTri
{
	int mSubMtrl = -1;
	PNCT_VERTEX v[3];
	inline bool operator< (const VertexTri& rhs) const
	{
		return mSubMtrl < rhs.mSubMtrl;
	}
};

struct SceneInfo
{
	int FirstFrame = -1;
	int LastFrame = -1;
	int FrameSpeed = -1;
	int TickperFrame = -1;
};

struct ZXCTexmap
{
	int SubNo = -1;

	std::wstring Filename;
};

struct ZXCMaterial
{
	std::wstring Name;
	std::wstring ClassName;

	std::vector<ZXCTexmap>	 TexMap;
	std::vector<ZXCMaterial> SubMaterial;
};

struct AnimTrack
{
	int mTick = 0;
	Quat mQuat = { 0.0f, 0.0f, 0.0f, 0.0f };
	Point3 mVec = { 0.0f, 0.0f, 0.0f };
};

struct ZXCObject
{
	int mMaterialRef = -1;
	std::wstring mNodeName;
	std::wstring mParentName;

	Box3 mBoundingBox;

	std::vector<AnimTrack>	mPosTrack;
	std::vector<AnimTrack>	mRotTrack;
	std::vector<AnimTrack>	mScaleTrack;

	std::vector<VertexTri> mTriangles;

	D3D_MATRIX mWorld;
	std::vector<ZXCObject> mSubMesh;
};

namespace std
{
	static inline void ReplaceString(wstring& src, const wstring& findString = L" ", const wstring replaceString = L"")
	{
		size_t pos = 0;
		while ((pos = src.find(findString)) != wstring::npos)
			src.replace(pos, findString.length(), replaceString);
	}
}

static inline Quat operator-(Quat& lhs)
{
	Quat temp = lhs;
	temp.x *= -1;
	temp.y *= -1;
	temp.z *= -1;
	temp.w *= -1;
	return temp;
}