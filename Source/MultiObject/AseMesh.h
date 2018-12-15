#pragma once

#include "Define.h"
#include "AseParser.h"
#include <DirectXMath.h>
#include <DirectXCollision.h>

class Mesh;

struct sScene
{
	int FirstFrame;
	int LastFrame;
	int FrameSpeed;
	int TicksperFrame;
};

struct TexMap
{
	int SubNo = -1;
	std::wstring Filename = std::wstring();
	std::wstring Filepath = std::wstring();
	inline bool empty()
	{
		if (SubNo == -1 && Filename.empty() && Filepath.empty())
		{
			return true;
		}
		return false;
	}
};

struct MaterialList
{
	std::wstring Name;
	std::wstring ClassName;
	std::vector<MaterialList> SubMaterial;
	std::vector<TexMap> Texture;
};

struct xIndex
{
	union
	{
		struct { UINT i0, i1, i2; };
		UINT v[3];
	};
	UINT Mtrl;
};

typedef struct AnimationTrack
{
	int mTick = 0;
	DirectX::XMFLOAT4 mQuatRotation;
	DirectX::XMFLOAT3 mPosition;
}AniTrack;

enum class ObjectType : unsigned char
{
	BONE = 0,
	DUMMY,
	GEOMESH
};

struct Helper
{
	std::string Name;
	std::string ParentName;

	std::vector<AniTrack> m_Position;
	std::vector<AniTrack> m_Rotation;
	std::vector<AniTrack> m_Scale;

	ObjectType m_ObjType = ObjectType::BONE;
	DirectX::BoundingBox m_BoundingBox;

	DirectX::XMFLOAT4X4 matWorld;
	DirectX::XMFLOAT3 Position;
	DirectX::XMFLOAT4 Rotation;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT4 QuatScale;
};

struct GeomMesh
{
	int mtrlRef;
	std::wstring Name;
	std::string ParentName = std::string();

	std::vector<DirectX::XMFLOAT3> vertexList;
	std::vector<xIndex> posFaceList;
	std::vector<DirectX::XMFLOAT2> texList;
	std::vector<xIndex> texFaceList;
	std::vector<DirectX::XMFLOAT4> colorList;
	std::vector<xIndex> colorFaceList;

	std::vector<DirectX::XMFLOAT3> norList;

	Helper m_Helper;
	std::unique_ptr<Helper> m_AniHelper = nullptr;
};


class AseMesh
{
	friend class AseParser;
public:
	AseMesh() = default;

public:
	bool Load(const std::wstring& name);
	std::unique_ptr<Mesh> Convert(ID3D11Device* device);
public:
	sScene m_Scene;
	std::vector<MaterialList> m_MateriaList;
	std::vector<GeomMesh> m_ObjectList;
	std::vector<Helper> m_HelperList;
};