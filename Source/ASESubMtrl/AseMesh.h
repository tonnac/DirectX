#pragma once

#include "AseParser.h"
#include <DirectXMath.h>

struct sScene
{
	int FirstFrame;
	int LastFrame;
	int FrameSpeed;
	int TicksperFrame;
};

struct TexMap
{
	std::wstring Filename;
	std::wstring Filepath;
};

struct MaterialList
{
	int SubNo;
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

struct GeomMesh
{
	int mtrlRef;
	std::wstring Name;
	DirectX::XMFLOAT4X4 matWorld;
	std::vector<DirectX::XMFLOAT3> vertexList;
	std::vector<xIndex> posFaceList;
	std::vector<DirectX::XMFLOAT2> texList;
	std::vector<xIndex> texFaceList;
	std::vector<DirectX::XMFLOAT4> colorList;
	std::vector<xIndex> colorFaceList;

	std::vector<DirectX::XMFLOAT3> norList;
};

class AseMesh
{
	friend class AseParser;
public:
	AseMesh() = default;

public:
	bool Load(const std::wstring& name);

public:
	sScene m_Scene;
	std::vector<MaterialList> m_MateriaList;
	std::vector<GeomMesh> m_ObjectList;
};