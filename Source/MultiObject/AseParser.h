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
struct AseConverter;
struct MaterialList;
struct GeomMesh;
struct Helper;

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
	NODE_PARENT,
	NODE_TM,
	MESH,
	TM_ANIMATION,
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

enum class HelperType : unsigned char
{
	NODE_NAME = 0,
	NODE_PARENT,
	HELPER_CLASS,
	NODE_TM,
	BOUNDINGBOX_MIN,
	TM_ANIMATION,
	Count
};

enum class TexType : unsigned char
{
	MAP_SUBNO = 0,
	FILE_PATH,
	Count
};

enum class MatrixType : unsigned char
{
	TM_ROW = 0,
	TM_POS,
	TM_ROTAXIS,
	TM_SCALE,
	Count
};

enum class AnimationType : unsigned char
{
	CONTROL_POS = 0,
	CONTROL_ROT,
	CONTROL_SCALE,
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
	void InputHelper(size_t HelperIndex, HelperType helperType);

	void InputMatrix(Helper* helper);
	void InputAnimation(Helper* helper);
	void InputPosTrack(Helper* helper);
	void InputRotTrack(Helper* helper);
	void InputScaleTrack(Helper* helper);
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
	static const std::array<std::string, 6> m_GeomeshType;
	static const std::array<std::string, 6> m_HelperType;
	static const std::array<std::string, 2> m_TextureType;
	static const std::array<std::string, 4> m_MatrixType;
	static const std::array<std::string, 3> m_AnimationType;
	const std::string m_EndText = "}";
};