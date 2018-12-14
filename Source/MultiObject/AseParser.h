#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <Windows.h>
#include <memory>
#include <functional>
#include <map>

class AseMesh;
struct MaterialList;
struct GeomMesh;

enum class SceneType : unsigned char
{
	FIRSTFRAME = 0,
	LASTFRAME,
	FRAMESPEED,
	TICKSPERFRAME,
	Count
};

enum class MaterialType : unsigned char
{
	MATERIAL_NAME = 0,
	MATERIAL_CLASS,
	MAP_DIFFUSE,
	MAP_REFLECT,
	Count
};

enum class MeshType : unsigned char
{
	NODE_NAME = 0,
	NODE_TM,
	MESH,
	MATERIAL_REF,
	Count
};

enum class VertexType : unsigned char
{
	MESH_NUMVERTEX = 0,
	MESH_NUMTVERTEX,
	MESH_NUMCVERTEX,
	MESH_NORMALS,
	Count
};

enum class TexType : unsigned char
{
	MAP_SUBNO = 0,
	FILE_PATH,
	Count
};

class AseParser
{
public:
	AseParser() = default;

public:
	bool LoadAse(const std::wstring& FileName, AseMesh* mesh);

private:
	void LoadScene();
	void LoadMaterial();
	void LoadSubMaterial(MaterialList* material);
	void LoadGeomesh(size_t index, size_t meshIndex);
	void LoadHelperObject(size_t index, size_t helperIndex);

	void InputScene(SceneType scene);
	void InputMaterial(MaterialList* material, MaterialType& materialType);
	void InputMap(MaterialList* material);
	void InputMesh(size_t GeomeshIndex, MeshType GeomeshType);

	void InputMatrix(size_t GeomeshIndex);
	void InputVertexData(size_t GeomeshIndex);

	void InputVertex(GeomMesh * mesh);
	void InputTexutre(GeomMesh * mesh);
	void InputColor(GeomMesh * mesh);
	void InputNormal(GeomMesh * mesh);

	void Findstring(const std::string& text);

	template <typename X>
	bool FindType(const std::string* strarr, X& enumtype)
	{
		int findType = -1;

		while (1)
		{
			for (;(int)m_Stream[m_Index].find(m_EndText) < 0; ++m_Index)
			{
				for (X i = (X)0; i < X::Count; IncreaseEnum(i, true))
				{
					findType = (int)m_Stream[m_Index].find(strarr[(int)i]);
					if (findType >= 0)
					{
						enumtype = i;
						return true;
					}
				}
			}
			break;
		}
		return false;
	}
private:
	std::vector<std::string> m_Stream;

	AseMesh * m_aseMesh = nullptr;

	size_t m_ScnenIndex = 0;
	size_t m_MaterialIndex = 0;
	size_t m_Index = 0;

	size_t m_GeomeshSize = 0;
	size_t m_HelperObjSize = 0;
	std::map<size_t, bool> m_ObjectIndex;

	static const std::array<std::string, 4> m_Type;
	static const std::array<std::string, 4> m_SceneType;
	static const std::array<std::string, 4> m_MaterialType;
	static const std::array<std::string, 4> m_GeomeshType;
	static const std::array<std::string, 2> m_TextureType;
	const std::string m_EndText = "}";
};