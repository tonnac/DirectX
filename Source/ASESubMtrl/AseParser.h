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
	MAP_SUBNO,
	TEX_FILE,
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
	void LoadGeomesh();

	void InputScene(SceneType scene);
	void InputMaterial(MaterialList* material, MaterialType& materialType);
	void InputMesh(size_t GeomeshIndex, MeshType GeomeshType);

	void InputMatrix(size_t GeomeshIndex);
	void InputVertexData(size_t GeomeshIndex);

	void InputVertex(GeomMesh * mesh);
	void InputTexutre(GeomMesh * mesh);
	void InputColor(GeomMesh * mesh);
	void InputNormal(GeomMesh * mesh);

	void Findstring(const std::string& text);
private:
	std::vector<std::string> m_Stream;

	AseMesh * m_aseMesh = nullptr;

	size_t m_ScnenIndex = 0;
	size_t m_MaterialIndex = 0;
	size_t m_Index = 0;
	std::vector<size_t> m_GeomeshIndex;

	static const std::array<std::string, 3> m_Type;
	static const std::array<std::string, 4> m_SceneType;
	static const std::array<std::string, 4> m_MaterialType;
	static const std::array<std::string, 4> m_GeomeshType;

	std::function<bool(const std::string&, int&)> m_func = [this](const std::string& text, int& num) -> bool
	{
		for (size_t i = 0; i < m_Type.size(); ++i)
		{
			int findType = (int)text.find(m_Type[i]);
			if (findType >= 0)
			{
				num = (int)i;
				return true;
			}
		}
		num = -1;
		return false;
	};
};