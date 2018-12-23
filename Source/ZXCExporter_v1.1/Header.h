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
#include <array>
#include <queue>

constexpr float Epsilon = 1.0e-3f;
#define MAXWEIGHTNUM 4

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

struct Vertex
{
	Point3 p;
	Point3 n;
	Point4 c = { 1.0f, 1.0f, 1.0f, 1.0f };
	Point2 t;
	int VertexNum = -1;
	inline bool operator == (const Vertex& rhs)
	{
		if (p.Equals(rhs.p, Epsilon) && n.Equals(rhs.n, Epsilon) && c.Equals(rhs.c, Epsilon) && t.Equals(rhs.t, Epsilon))
		{
			return true;
		}
		return false;
	}
};

struct PNCTW4VERTEX : public Vertex
{
	std::array<float, 4> w;
	std::array<std::wstring, 4> i;
};

struct PNCTW8VERTEX : public Vertex
{
	std::array<float, 4> w0;
	std::array<std::wstring, 4> i0;
	std::array<float, 4> w1;
	std::array<std::wstring, 4> i1;
};

struct VertexTri
{
	int mSubMtrl = -1;
	Vertex v[3];
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

struct KeyFrame
{
	KeyFrame() {}
	~KeyFrame() {}

	int Tick;
	Point3 Translations;
	Point3 Scale;
	Quat ScaleQuat;
	Quat RotationQuat;
};

struct BoneAnimation
{
	std::vector<KeyFrame> Keyframes;
};

struct AnimationClip
{
	std::vector<BoneAnimation> BoneAnimations;
};

enum class ObjectType
{
	MESH,
	DUMMY,
	BONE,
	BIPED,
};

struct ZXCObject
{
	int mMaterialRef = -1;

	std::pair<std::wstring, size_t> mNodeName;
	std::pair<std::wstring, size_t> mParentName;

	ObjectType Type = ObjectType::MESH;

	Box3 mBoundingBox;
	AffineParts InititalPos;

	D3D_MATRIX mWorld;
	std::unordered_map<std::string, AnimationClip> mAnimations;

	std::vector<VertexTri> mTriangles;

	int MeshNum = -1;
	std::map<int, std::vector<Vertex>> mVertices;
	std::map<int, std::vector<std::uint32_t>> mIndices;
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

class MaxUtil
{
public:
	template<typename X>
	static void ConvertVector(const X& src, X& dest)
	{
		X copysrc = src;
		copysrc.y = src.z;
		copysrc.z = src.y;
		CopyMemory(&dest, &copysrc, sizeof(X));
	}

	static void ConvertMatrix(const Matrix3& maxMatrix, D3D_MATRIX& dxMatrix)
	{
		Point3 row0, row1, row2, row3;
		row0 = maxMatrix.GetRow(0);
		row1 = maxMatrix.GetRow(1);
		row2 = maxMatrix.GetRow(2);
		row3 = maxMatrix.GetRow(3);

		dxMatrix.m[0][0] = row0.x, dxMatrix.m[0][1] = row0.z, dxMatrix.m[0][2] = row0.y;
		dxMatrix.m[1][0] = row2.x, dxMatrix.m[1][1] = row2.z, dxMatrix.m[1][2] = row2.y;
		dxMatrix.m[2][0] = row1.x, dxMatrix.m[2][1] = row1.z, dxMatrix.m[2][2] = row1.y;
		dxMatrix.m[3][0] = row3.x, dxMatrix.m[3][1] = row3.z, dxMatrix.m[3][2] = row3.y;
	}

	static bool isTMNegative(const Matrix3& m)
	{
		return (DotProd(CrossProd(m.GetRow(0), m.GetRow(1)), m.GetRow(2)) < 0.0f) ? true : false;
	}

	static bool isBipedObject(INode* node)
	{
		Modifier * phyMod = FindModifer(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
		Modifier * skinMod = FindModifer(node, SKIN_CLASSID);

		if (phyMod != nullptr || skinMod != nullptr)
		{
			return true;
		}

		return false;
	}

	static Modifier* FindModifer(INode * node, Class_ID classID)
	{
		Object * object = node->GetObjectRef();
		if (object == nullptr) return nullptr;

		while (object->SuperClassID() == GEN_DERIVOB_CLASS_ID && object)
		{
			IDerivedObject* DerivedObject = static_cast<IDerivedObject*>(object);

			int modStackIndex = 0;

			while (modStackIndex < DerivedObject->NumModifiers())
			{
				Modifier * modifier = DerivedObject->GetModifier(modStackIndex);

				if (modifier->ClassID() == classID)
					return modifier;

				++modStackIndex;
			}
			object = DerivedObject->GetObjRef();
		}
		return nullptr;
	}
};

static inline Quat operator-(Quat& lhs)
{
	Quat temp = lhs;
	temp.x *= -1;
	temp.y *= -1;
	temp.z *= -1;
	temp.w *= -1;
	return temp;
}