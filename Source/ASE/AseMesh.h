#pragma once

#define _CRT_SECURE_NO_WARNINGS
#include "Define.h"

struct sScene
{
	int FirstFrame;
	int LastFrame;
	int FrameSpeed;
	int TicksperFrame;
};

struct TexMap
{
	int iType;
	std::tstring Name;
};

struct MaterialList
{
	std::tstring Name;
	int iType;
	std::vector<MaterialList> SubMaterial;
	std::vector<TexMap> Texture;
};

struct xIndex
{
	DWORD i0;
	DWORD i1;
	DWORD i2;
};

struct GeomMesh
{
	int mtrlRef;
	std::tstring Name;
	D3DXMATRIX matWorld;
	std::vector<D3DXVECTOR3> vertexList;
	std::vector<xIndex> posFaceList;
	std::vector<D3DXVECTOR3> texList;
	std::vector<xIndex> texFaceList;
	std::vector<D3DXVECTOR3> colorList;
	std::vector<xIndex> colorFaceList;

	std::vector<D3DXVECTOR3> norList;
};

class AseMesh
{
public:
	sScene m_Scene;
	std::vector<MaterialList> m_MateriaList;
	std::vector<GeomMesh> m_ObjectList;

	TCHAR m_pBuffer[256] = { 0, };
	TCHAR m_pString[256] = { 0, };
	FILE* m_pStream;
public:
	bool Load(std::tstring name);
	bool LoadScene();
public:

};