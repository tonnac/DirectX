#include "AseParser.h"
#include "AseMesh.h"
#include "DxState.h"

using namespace DirectX;

const std::array<std::string, 4> AseParser::m_Type =
{
	"*SCENE {",
	"*MATERIAL_LIST {",
	"*HELPEROBJECT {",
	"*GEOMOBJECT {"
};

const std::array<std::string, 4> AseParser::m_SceneType =
{
	"*SCENE_FIRSTFRAME",
	"*SCENE_LASTFRAME",
	"*SCENE_FRAMESPEED",
	"*SCENE_TICKSPERFRAME"
};

const std::array<std::string, 4> AseParser::m_MaterialType =
{
	"*MATERIAL_NAME",
	"*MATERIAL_CLASS",
	"*MAP_DIFFUSE",
	"*MAP_REFLECT",
};

const std::array<std::string, 4> AseParser::m_GeomeshType =
{
	"*NODE_NAME",
	"*NODE_TM {",
	"*MESH {",
	"*MATERIAL_REF"
};

const std::array<std::string, 2> AseParser::m_TextureType =
{
	"*MAP_SUBNO",
	"\\",
};

bool AseParser::LoadAse(const std::wstring & FileName, AseMesh * mesh)
{
	m_aseMesh = mesh;

	std::ifstream fp(FileName.c_str());
	std::string buffer;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L"Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	size_t k = 0;
	while (!fp.eof())
	{
		std::getline(fp, buffer);
		for (size_t i = 0; i < m_Type.size(); ++i)
		{
			int findType = (int)buffer.find(m_Type[i]);
			if (findType >= 0)
			{
				k = i;
				break;
			}
			k = -1;
		}

		switch (k)
		{
		case 0:
			m_ScnenIndex = m_Stream.size();
			break;
		case 1:
			m_MaterialIndex = m_Stream.size();
			break;
		case 2:
			m_ObjectIndex.insert(std::pair<size_t, bool>(m_Stream.size(), false));
			++m_HelperObjSize;
			break;
		case 3:
			m_ObjectIndex.insert(std::pair<size_t, bool>(m_Stream.size(), true));
			++m_GeomeshSize;
			break;
		}		

		m_Stream.push_back(buffer);
	}

	fp.close();

	m_aseMesh->m_ObjectList.resize(m_GeomeshSize);

	LoadScene();
	LoadMaterial();
	size_t meshindex = -1;
	size_t helperindex = -1;
	for (auto& x : m_ObjectIndex)
	{
		if (!x.second)
		{
			LoadHelperObject(x.first, ++helperindex);
		}
		else
		{
			LoadGeomesh(x.first, ++meshindex);
		}
	}

	return true;
}

void AseParser::LoadScene()
{
	SceneType scene = SceneType::FIRSTFRAME;
	m_Index = m_ScnenIndex;

	while (m_Index != m_MaterialIndex && (int)scene != m_SceneType.size())
	{
		Findstring(m_SceneType[(int)scene]);
		InputScene(scene);
		IncreaseEnum(scene, true);
	}
}

void AseParser::LoadMaterial()
{
	size_t Cnt = 0;
	MaterialType mtrl = (MaterialType)0;
	m_Index = m_MaterialIndex + 1;

	std::string buffer;
	int MatirialCount = 0;

	std::istringstream is(m_Stream[m_Index]);
	is >> buffer >> MatirialCount;
	m_aseMesh->m_MateriaList.resize(MatirialCount);

	while (Cnt < MatirialCount)
	{
		Findstring("*MATERIAL ");
		MaterialList * materialList = &m_aseMesh->m_MateriaList[Cnt];

		for (;FindType(m_MaterialType.data(), mtrl); ++m_Index)
		{
			InputMaterial(materialList, mtrl);
		}
		mtrl = (MaterialType)0;
		++Cnt;
	}
}

void AseParser::LoadSubMaterial(MaterialList* material)
{
	std::string buffer;
	MaterialType mtrl = (MaterialType)0;
	int Cnt = 0;
	int SubMaterialNum = 0;
	Findstring("*NUMSUBMTLS");
	std::istringstream is(m_Stream[m_Index]);
	is >> buffer >> SubMaterialNum;

	material->SubMaterial.resize(SubMaterialNum);

	while (Cnt < SubMaterialNum)
	{
		Findstring("*SUBMATERIAL");
		for (;FindType(m_MaterialType.data(), mtrl); ++m_Index)
		{
			InputMaterial(&material->SubMaterial[Cnt], mtrl);
		}
		mtrl = (MaterialType)0;
		++Cnt;
	}
}

void AseParser::LoadGeomesh(size_t index, size_t meshIndex)
{
	m_Index = index + 1;

	MeshType meshtype = (MeshType)0;

	while ((int)meshtype != m_GeomeshType.size())
	{
		Findstring(m_GeomeshType[(int)meshtype]);
		InputMesh(meshIndex, meshtype);
		IncreaseEnum(meshtype, true);
	}
}

void AseParser::LoadHelperObject(size_t index, size_t helperIndex)
{
}

void AseParser::InputScene(SceneType scene)
{
	std::string buffer;
	std::istringstream is(m_Stream[m_Index]);
	switch (scene)
	{
	case SceneType::FIRSTFRAME:
		is >> buffer >> m_aseMesh->m_Scene.FirstFrame;
		break;
	case SceneType::LASTFRAME:
		is >> buffer >> m_aseMesh->m_Scene.LastFrame;
		break;
	case SceneType::FRAMESPEED:
		is >> buffer >> m_aseMesh->m_Scene.FrameSpeed;
		break;
	case SceneType::TICKSPERFRAME:
		is >> buffer >> m_aseMesh->m_Scene.TicksperFrame;
		break;
	default:
		break;
	}
}
void AseParser::InputMaterial(MaterialList* material, MaterialType& materialType)
{
	std::istringstream is(m_Stream[m_Index]);
	std::string buffer;

	switch (materialType)
	{
	case MaterialType::MATERIAL_NAME:
	{
		is >> buffer;
		std::getline(is, buffer);
		std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
		material->Name = std::wstring(name.begin(), name.end());
	}
	break;
	case MaterialType::MATERIAL_CLASS:
	{
		is >> buffer;
		std::getline(is, buffer);
		std::string classname(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
		material->ClassName = std::wstring(classname.begin(), classname.end());
		int findMulti = (int)classname.find("Multi/Sub-Object");
		if (findMulti >= 0)
		{
			LoadSubMaterial(material);
			materialType = MaterialType::Count;
		}
	}
	break;
	case MaterialType::MAP_DIFFUSE:
	case MaterialType::MAP_REFLECT:
	{
		InputMap(material);
	}
	break;
	}
}

void AseParser::InputMap(MaterialList * material)
{
	int Num = 0;
	TexType textype = (TexType)0;
	TexMap Tex;
	for (;FindType(m_TextureType.data(), textype); ++m_Index)
	{
		std::istringstream is(m_Stream[m_Index]);
		std::string buffer;
		switch (textype)
		{
			case TexType::MAP_SUBNO:
			{
				is >> buffer >> Tex.SubNo;
			}
			break;
			case TexType::FILE_PATH:
			{
				is >> buffer;
				std::getline(is, buffer);
				std::string filepath(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('\\') - 1);
				std::string filename(buffer, buffer.find_last_of('\\') + 1, buffer.find_last_of('"') - 1 - buffer.find_last_of('\\'));
				Tex.Filename = std::wstring(filename.begin(), filename.end());
				Tex.Filepath = std::wstring(filepath.begin(), filepath.end());
			}
			break;
		}
	}
	if(!Tex.empty())
		material->Texture.push_back(Tex);
}

void AseParser::InputMesh(size_t GeomeshIndex, MeshType GeomeshType)
{
	switch (GeomeshType)
	{
		case MeshType::NODE_NAME:
		{
			std::string buffer;
			std::istringstream is(m_Stream[m_Index]);
			is >> buffer;
			std::getline(is, buffer);
			std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
			m_aseMesh->m_ObjectList[GeomeshIndex].Name = std::wstring(name.begin(), name.end());
		}
		break;
		case MeshType::NODE_TM:
		{
			InputMatrix(GeomeshIndex);
		}
		break;
		case MeshType::MESH:
		{
			InputVertexData(GeomeshIndex);
		}
		break;
		case MeshType::MATERIAL_REF:
		{
			std::istringstream is(m_Stream[m_Index]);
			std::string buffer;
			is >> buffer >> m_aseMesh->m_ObjectList[GeomeshIndex].mtrlRef;
		}
		break;
	}
}

void AseParser::InputMatrix(size_t GeomeshIndex)
{
	std::string buffer;

	Findstring("*TM_ROW0");

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixIdentity());

	for (int i = 0; i < 4; ++i)
	{
		std::istringstream is(m_Stream[m_Index++]);
		is >> buffer >> world.m[i][0] >> world.m[i][2] >> world.m[i][1];
	}
	float e[4];
	CopyMemory(e, world.m[1], sizeof(float) * 4);
	CopyMemory(world.m[1], world.m[2], sizeof(float) * 4);
	CopyMemory(world.m[2], e, sizeof(float) * 4);

	m_aseMesh->m_ObjectList[GeomeshIndex].matWorld = world;
}

void AseParser::InputVertexData(size_t GeomeshIndex)
{
	static const std::array<std::string, 4> vertexType = 
	{ 
		"*MESH_NUMVERTEX",
		"*MESH_NUMTVERTEX",
		"*MESH_NUMCVERTEX",
		"*MESH_NORMALS"
	};

 	auto& ObjList = m_aseMesh->m_ObjectList[GeomeshIndex];

	for (VertexType vType = VertexType::MESH_NUMVERTEX; vType < VertexType::Count; IncreaseEnum(vType, true))
	{
		Findstring(vertexType[(int)vType]);
		switch (vType)
		{
		case VertexType::MESH_NUMVERTEX:
			InputVertex(&ObjList);
			break;
		case VertexType::MESH_NUMTVERTEX:
			InputTexutre(&ObjList);
			break;
		case VertexType::MESH_NUMCVERTEX:
			InputColor(&ObjList);
			break;
		case VertexType::MESH_NORMALS:
			InputNormal(&ObjList);
			break;
		}
	}
}

void AseParser::InputVertex(GeomMesh * mesh)
{
	std::string ignore;
	int Num;

	std::istringstream is(m_Stream[m_Index++]);
	is >> ignore >> Num;

	mesh->vertexList.resize(Num);

	is.clear();
	is.str(m_Stream[m_Index++]);
	is >> ignore >> Num;

	mesh->posFaceList.resize(Num);
	++m_Index;

	for (size_t i = 0; i < mesh->vertexList.size(); ++i, ++m_Index)
	{
		is.clear();
		is.str(m_Stream[m_Index]);
		is >> ignore >> ignore >> mesh->vertexList[i].x >> mesh->vertexList[i].z >> mesh->vertexList[i].y;
	}

	m_Index += 2;

	for (size_t i = 0; i < mesh->posFaceList.size(); ++i, ++m_Index)
	{
		xIndex index;
		is.clear();
		is.str(m_Stream[m_Index]);
		is >> ignore >> ignore >> ignore >> index.i0 >> ignore >> index.i2 >> ignore >> index.i1;
		for (int k = 0; k < 8; ++k)
		{
			is >> ignore;
		}
		is >> ignore >> index.Mtrl;
		mesh->posFaceList[i] = index;
	}
}

void AseParser::InputTexutre(GeomMesh * mesh)
{
	std::string ignore;
	int Num;
	std::istringstream is(m_Stream[m_Index++]);
	
	is >> ignore >> Num;
	mesh->texList.resize(Num);

	++m_Index;

	for (size_t i = 0; i < mesh->texList.size(); ++i, ++m_Index)
	{
		float v;
		is.clear();
		is.str(m_Stream[m_Index]);
		is >> ignore >> ignore >> mesh->texList[i].x >> v;
		mesh->texList[i].y = 1.0f - v;
	}

	is.clear();
	is.str(m_Stream[++m_Index]);
	is >> ignore >> Num;

	mesh->texFaceList.resize(Num);

	m_Index += 2;

	for (size_t i = 0; i < mesh->texFaceList.size(); ++i, ++m_Index)
	{
		xIndex index;
		is.clear();
		is.str(m_Stream[m_Index]);
		is >> ignore >> ignore >> index.i0 >> index.i2 >> index.i1;
		mesh->texFaceList[i] = index;
	}
}

void AseParser::InputColor(GeomMesh * mesh)
{
	std::string ignore;
	int Num;
	std::istringstream is(m_Stream[m_Index++]);
	is >> ignore >> Num;

	mesh->colorList.resize(Num);

	if (Num == 0)
	{
		return;
	}

	++m_Index;

	for (size_t i = 0; i < mesh->colorList.size(); ++i, ++m_Index)
	{
		is.clear();
		is.str(m_Stream[m_Index]);
		is >> ignore >> ignore >> mesh->colorList[i].x >> mesh->colorList[i].y >> mesh->colorList[i].z;
	}

	is.clear();
	is.str(m_Stream[++m_Index]);
	is >> ignore >> Num;

	mesh->colorFaceList.resize(Num);

	m_Index += 2;

	for (size_t i = 0; i < mesh->colorFaceList.size(); ++i, ++m_Index)
	{
		is.clear();
		is.str(m_Stream[m_Index]);
		is >> ignore >> ignore >> mesh->colorFaceList[i].i0 >> mesh->colorFaceList[i].i2 >> mesh->colorFaceList[i].i1;
	}
}

void AseParser::InputNormal(GeomMesh * mesh)
{
	std::string ignore;
	int Num;
	std::istringstream is(m_Stream[m_Index++]);
	is >> ignore >> Num;

	mesh->norList.resize(mesh->posFaceList.size() * 3);

	++m_Index;

	int t = -1;
	for (size_t i = 0; i < mesh->posFaceList.size() * 4; ++i, ++m_Index)
	{
		if (i % 4 == 0)
			continue;
		is.str(m_Stream[m_Index]);
		is.clear();
		is >> ignore >> ignore >> mesh->norList[++t].x >> mesh->norList[t].z >> mesh->norList[t].y;
	}
}

void AseParser::Findstring(const std::string & text)
{
	int findText = (int)m_Stream[m_Index].find(text);
	while (findText < 0)
	{
		findText = (int)m_Stream[++m_Index].find(text);
	}
}