#include "AseParser.h"
#include "AseMesh.h"

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
	size_t k = 0;
	size_t index = m_ScnenIndex;

	while (index != m_MaterialIndex && k != m_SceneType.size())
	{
		int findType = (int)m_Stream[index].find(m_SceneType[k]);
		if (findType >= 0)
		{
			std::istringstream is(m_Stream[index]);
			InputScene(is, k++);
		}
		++index;
	}
}

void AseParser::LoadMaterial()
{
	size_t k = 0;
	size_t Cnt = 0;
	size_t index = m_MaterialIndex + 1;

	std::string buffer;
	int MatirialCount = 0;

	std::istringstream is0(m_Stream[index]);
	is0 >> buffer >> MatirialCount;
	m_aseMesh->m_MateriaList.resize(MatirialCount);

	while (Cnt < MatirialCount)
	{
		MaterialList * materialList = &m_aseMesh->m_MateriaList[Cnt];
		while (index != m_GeomeshIndex[0] && k != m_MaterialType.size())
		{
			int findType = (int)m_Stream[index].find(m_MaterialType[k]);
			if (findType >= 0)
			{
				std::istringstream is(m_Stream[index]);
				InputMaterial(is, index, materialList, k++);
			}
			++index;
		}
		k = 0;
		++Cnt;
	}
}

void AseParser::LoadSubMaterial(size_t & streamIndex, MaterialList* material)
{
	std::string buffer;
	int k = 0;
	int SubMaterialNum = 0;
	Findstring(streamIndex, "*NUMSUBMTLS");
	std::istringstream is(m_Stream[streamIndex]);
	is >> buffer >> SubMaterialNum;

	material->SubMaterial.resize(SubMaterialNum);

	for (int i = 0; i < SubMaterialNum; ++i)
	{
		int findType = m_Stream[streamIndex].find(m_MaterialType[k]);
		if (findType >= 0)
		{
			InputMaterial(is, streamIndex, &material->SubMaterial[i], k);
		}
	}
}

void AseParser::LoadGeomesh()
{
	m_aseMesh->m_ObjectList.resize(m_GeomeshIndex.size());

	size_t k = 0;
	size_t Cnt = 0;
	size_t index = m_GeomeshIndex[0] + 1;

	std::string buffer;
	size_t GeomeshCount = m_GeomeshIndex.size();

	while (Cnt < GeomeshCount)
	{
		while (m_Stream[index - 1] != "*MATERIAL_REF" && k != m_GeomeshType.size())
		{
			int findType = (int)m_Stream[index].find(m_GeomeshType[k]);
			if (findType >= 0)
			{
				InputGeomesh(index, Cnt, k++);
			}
			++index;
		}
		k = 0;
		++Cnt;
	}
}

void AseParser::InputScene(std::istringstream& is, size_t SceneTypeIndex)
{
	std::string buffer;
	switch (SceneTypeIndex)
	{
	case 0:
		is >> buffer >> m_aseMesh->m_Scene.FirstFrame;
		break;
	case 1:
		is >> buffer >> m_aseMesh->m_Scene.LastFrame;
		break;
	case 2:
		is >> buffer >> m_aseMesh->m_Scene.FrameSpeed;
		break;
	case 3:
		is >> buffer >> m_aseMesh->m_Scene.TicksperFrame;
		break;
	}
}
void AseParser::InputMaterial(std::istringstream& is, size_t& streamIndex, MaterialList* material, size_t MaterialType)
{
	std::string buffer;
	switch (MaterialType)
	{
		case 0:
		{
			is >> buffer;
			std::getline(is, buffer);
			std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
			material->Name = std::wstring(name.begin(), name.end());
		}
		break;
		case 1:
		{
			is >> buffer;
			std::getline(is, buffer);
			std::string classname(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
			material->ClassName = std::wstring(classname.begin(), classname.end());
			int findMulti = (int)classname.find("Multi/Sub-Object");
			if(findMulti >= 0)
				LoadSubMaterial(streamIndex, material);
		}
		break;
		case 2:
		{
			is >> buffer >> material->SubNo;
		}
		break;
		case 3:
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

void AseParser::InputGeomesh(size_t& streamIndex, size_t GeomeshIndex, size_t GeomeshType)
{
	switch (GeomeshType)
	{
		case 0:
		{
			std::string buffer;
			std::istringstream is(m_Stream[streamIndex]);
			is >> buffer;
			std::getline(is, buffer);
			std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
			m_aseMesh->m_ObjectList[GeomeshIndex].Name = std::wstring(name.begin(), name.end());
		}
		break;
		case 1:
		{
			InputMatrix(streamIndex, GeomeshIndex, GeomeshType);
		}
		break;
		case 2:
		{
			InputVertex(streamIndex, GeomeshIndex, GeomeshType);
		}
		break;
		case 3:
		{
			std::istringstream is(m_Stream[streamIndex]);
			std::string buffer;
			is >> buffer >> m_aseMesh->m_ObjectList[GeomeshIndex].mtrlRef;
		}
		break;
	}
}

void AseParser::InputMatrix(size_t& streamIndex, size_t GeomeshIndex, size_t GeomeshType)
{
	static const std::array<std::string, 1> matrixType = { "*TM_ROW0" };

	size_t k = 0;
	std::string buffer;
	while (m_Stream[streamIndex] != m_GeomeshType[GeomeshType + 1])
	{
		int findType = (int)m_Stream[streamIndex].find(matrixType[k]);
		if (findType >= 0)
		{
			XMFLOAT4X4 world;
			XMStoreFloat4x4(&world, XMMatrixIdentity());

			for (int i = 0; i < 4; ++i)
			{
				std::istringstream is(m_Stream[streamIndex++]);
				is >> buffer >> world.m[i][0] >> world.m[i][2] >> world.m[i][1];
			}
			float e[4];
			CopyMemory(e, world.m[1], sizeof(float) * 4);
			CopyMemory(world.m[1], world.m[2], sizeof(float) * 4);
			CopyMemory(world.m[2], e, sizeof(float) * 4);

			m_aseMesh->m_ObjectList[GeomeshIndex].matWorld = world;
			break;
		}
		++streamIndex;
	}
	--streamIndex;
}

void AseParser::InputVertex(size_t& streamIndex, size_t GeomeshIndex, size_t GeomeshType)
{
	static const std::array<std::string, 1> vertexType = 
	{ 
		"*MESH_NUMVERTEX"
	};

	std::string ignore;
	std::string buffer[8];
	int Num;
	while ((int)m_Stream[streamIndex].find(m_GeomeshType[GeomeshType + 1]) < 0)
	{
		auto& ObjList = m_aseMesh->m_ObjectList[GeomeshIndex];


		streamIndex += 2;
		std::istringstream is(m_Stream[streamIndex++]);
		is >> ignore >> Num;

		ObjList.vertexList.resize(Num);

		is.clear();
		is.str(m_Stream[streamIndex++]);
		is >> ignore >> Num;

		ObjList.posFaceList.resize(Num);
		++streamIndex;

		for (size_t i = 0; i < ObjList.vertexList.size(); ++i, ++streamIndex)
		{
			is.clear();
			is.str(m_Stream[streamIndex]);
			is >> ignore >> ignore >> ObjList.vertexList[i].x >> ObjList.vertexList[i].z >> ObjList.vertexList[i].y;
		}

		streamIndex += 2;

		for (size_t i = 0; i < ObjList.posFaceList.size(); ++i, ++streamIndex)
		{
			xIndex index;
			is.clear();
			is.str(m_Stream[streamIndex]);
			is >> ignore >> ignore >> ignore >> index.i0 >> ignore >> index.i2 >> ignore >> index.i1;
			for (int k = 0; k < 8; ++k)
			{
				is >> ignore;
			}
			is >> ignore >> index.Mtrl;
			ObjList.posFaceList[i] = index;
		}

		is.seekg(0, std::ios::beg);
		is.clear();
		is.str(m_Stream[++streamIndex]);
		is >> ignore >> Num;
		ObjList.texList.resize(Num);

		streamIndex += 2;

		for (size_t i = 0; i < ObjList.texList.size(); ++i, ++streamIndex)
		{
			float v;
			is.clear();
			is.str(m_Stream[streamIndex]);
			is >> ignore >> ignore >> ObjList.texList[i].x >> v;
			ObjList.texList[i].y = 1.0f - v;
		}

		is.clear();
		is.str(m_Stream[++streamIndex]);
		is >> ignore >> Num;

		ObjList.texFaceList.resize(Num);

		streamIndex += 2;

		for (size_t i = 0; i < ObjList.texFaceList.size(); ++i, ++streamIndex)
		{
			xIndex index;
			is.clear();
			is.str(m_Stream[streamIndex]);
			is >> ignore >> ignore >> index.i0 >> index.i2 >> index.i1;
			ObjList.texFaceList[i] = index;
		}

		is.clear();
		is.str(m_Stream[++streamIndex]);
		is >> ignore >> Num;

		ObjList.colorList.resize(Num);

		for (size_t i = 0; i < ObjList.colorFaceList.size(); ++i, ++streamIndex)
		{

		}

		streamIndex += 2;

		ObjList.norList.resize(ObjList.posFaceList.size() * 3);
		int t = -1;
		for (size_t i = 0; i < ObjList.posFaceList.size() * 4; ++i, ++streamIndex)
		{
			if (i % 4 == 0)
				continue;
			is.str(m_Stream[streamIndex]);
			is.clear();
			is >> ignore >> ignore >> ObjList.norList[++t].x >> ObjList.norList[t].z >> ObjList.norList[t].y;
		}

		streamIndex += 5;
	}
	--streamIndex;
}

void AseParser::Findstring(size_t & streamIndex, const std::string & text)
{
	int findText = (int)m_Stream[streamIndex].find(text);
	while (findText < 0)
	{
		findText = (int)m_Stream[++streamIndex].find(text);
	}
}
