#include "AseParser.h"
#include "AseMesh.h"
#include "DxState.h"

using namespace DirectX;

const std::array<std::string, 3> AseParser::m_Type =
{
	"*SCENE {",
	"*MATERIAL_LIST {",
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
	"*MAP_SUBNO",
	"\\"
};

const std::array<std::string, 4> AseParser::m_GeomeshType =
{
	"*NODE_NAME",
	"*NODE_TM {",
	"*MESH {",
	"*MATERIAL_REF"
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
		int findType = (int)buffer.find(m_Type[k]);
		if (findType >= 0)
		{
			switch (k)
			{
			case 0:
				m_ScnenIndex = m_Stream.size();
				break;
			case 1:
				m_MaterialIndex = m_Stream.size();
				break;
			case 2:
				m_GeomeshIndex.push_back(m_Stream.size());
				break;
			}
			k = (k + 1 >= m_Type.size()) ? m_Type.size() - 1 : k + 1;
		}
		m_Stream.push_back(buffer);
	}
	
	fp.close();

	LoadScene();
	LoadMaterial();
	LoadGeomesh();

	return true;
}

void AseParser::LoadScene()
{
	SceneType scene = SceneType::FIRSTFRAME;
	m_Index = m_ScnenIndex;

	while (m_Index != m_MaterialIndex && (int)scene != m_SceneType.size())
	{
		int findType = (int)m_Stream[m_Index].find(m_SceneType[(int)scene]);
		if (findType >= 0)
		{
			InputScene(scene);
			IncreaseEnum(scene, true);
		}
		++m_Index;
	}
}

void AseParser::LoadMaterial()
{
	size_t Cnt = 0;
	MaterialType mtrl = MaterialType::MATERIAL_NAME;
	m_Index = m_MaterialIndex + 1;

	std::string buffer;
	int MatirialCount = 0;

	std::istringstream is(m_Stream[m_Index]);
	is >> buffer >> MatirialCount;
	m_aseMesh->m_MateriaList.resize(MatirialCount);

	while (Cnt < MatirialCount)
	{
		MaterialList * materialList = &m_aseMesh->m_MateriaList[Cnt];
		while (m_Index != m_GeomeshIndex[0] && (int)mtrl <= m_MaterialType.size())
		{
			Findstring(m_MaterialType[(int)mtrl]);
			InputMaterial(materialList, mtrl);
			IncreaseEnum(mtrl, true);
		}
		mtrl = MaterialType::MATERIAL_NAME;
		++Cnt;
	}
}

void AseParser::LoadSubMaterial(MaterialList* material)
{
	std::string buffer;
	MaterialType mtrl = MaterialType::MATERIAL_NAME;
	int Cnt = 0;
	int SubMaterialNum = 0;
	Findstring("*NUMSUBMTLS");
	std::istringstream is(m_Stream[m_Index]);
	is >> buffer >> SubMaterialNum;

	material->SubMaterial.resize(SubMaterialNum);

	while (Cnt < SubMaterialNum)
	{
		while ((int)mtrl != m_MaterialType.size())
		{
			Findstring(m_MaterialType[(int)mtrl]);
			InputMaterial(&material->SubMaterial[Cnt], mtrl);
			IncreaseEnum(mtrl, true);
		}
		++Cnt;
		mtrl = MaterialType::MATERIAL_NAME;
	}
}

void AseParser::LoadGeomesh()
{
	m_Index = m_GeomeshIndex[0] + 1;

	size_t GeomeshCount = m_GeomeshIndex.size();
	m_aseMesh->m_ObjectList.resize(GeomeshCount);
	size_t Cnt = 0;
	MeshType meshtype = MeshType::NODE_NAME;

	while (Cnt < GeomeshCount)
	{
		while ((int)meshtype != m_GeomeshType.size())
		{
			Findstring(m_GeomeshType[(int)meshtype]);
			InputMesh(Cnt, meshtype);
			IncreaseEnum(meshtype, true);
		}
		++Cnt;
		meshtype = MeshType::NODE_NAME;
	}
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
	case MaterialType::MAP_SUBNO:
	{
		is >> buffer >> material->SubNo;
	}
	break;
	case MaterialType::TEX_FILE:
	{
		is >> buffer;
		std::getline(is, buffer);
		std::string filepath(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('\\') - 1);
		std::string filename(buffer, buffer.find_last_of('\\') + 1, buffer.find_last_of('"') - 1 - buffer.find_last_of('\\'));
		TexMap tex;
		tex.Filename = std::wstring(filename.begin(), filename.end());
		tex.Filepath = std::wstring(filepath.begin(), filepath.end());
		material->Texture.push_back(tex);
	}
	break;
	}
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
