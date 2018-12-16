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

const std::array<std::string, 6> AseParser::m_GeomeshType =
{
	"*NODE_NAME",
	"*NODE_PARENT",
	"*NODE_TM {",
	"*MESH {",
	"*TM_ANIMATION {",
	"*MATERIAL_REF"
};

const std::array<std::string, 6> AseParser::m_HelperType =
{
	"*NODE_NAME",
	"*NODE_PARENT",
	"*HELPER_CLASS",
	"*NODE_TM {",
	"*BOUNDINGBOX_MIN",
	"*TM_ANIMATION",
};

const std::array<std::string, 2> AseParser::m_TextureType =
{
	"*MAP_SUBNO",
	"\\",
};

const std::array<std::string, 4> AseParser::m_MatrixType
{
	"*TM_ROW0",
	"*TM_POS",
	"*TM_ROTAXIS",
	"*TM_SCALE "
};

const std::array<std::string, 3> AseParser::m_AnimationType
{
	"*CONTROL_POS_TRACK",
	"*CONTROL_ROT_TRACK",
	"*CONTROL_SCALE_TRACK"
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
	m_aseMesh->m_HelperList.resize(m_HelperObjSize);

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

	for (;FindType(m_GeomeshType.data(), meshtype); ++m_Index)
	{
		InputMesh(meshIndex, meshtype);
	}
}

void AseParser::LoadHelperObject(size_t index, size_t helperIndex)
{
	m_Index = index + 1;

	HelperType helpertype = (HelperType)0;

	for (;FindType(m_HelperType.data(), helpertype); ++m_Index)
	{
		InputHelper(helperIndex, helpertype);
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
				std::string filepath(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('\\') - 2);
				std::string filename(buffer, buffer.find_last_of('\\') + 1, buffer.find_last_of('"') - 1 - buffer.find_last_of('\\'));
				std::string filePath0(filepath, filepath.find_last_of('\\') + 1, filepath.length());
				if ((int)filePath0.find("data") < 0)
				{
					filename = filePath0 + '\\' + filename;
				}
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
		case MeshType::NODE_PARENT:
		{
			std::string buffer;
			std::istringstream is(m_Stream[m_Index]);
			is >> buffer;
			std::getline(is, buffer);
			std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
			m_aseMesh->m_ObjectList[GeomeshIndex].ParentName = name;
		}
		break;
		case MeshType::NODE_TM:
		{
			InputMatrix(&m_aseMesh->m_ObjectList[GeomeshIndex].m_Helper);
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
		case MeshType::TM_ANIMATION:
		{
			std::unique_ptr<Helper> helper = std::make_unique<Helper>();
			helper->m_ObjType = ObjectType::GEOMESH;
			InputAnimation(helper.get());
			m_aseMesh->m_ObjectList[GeomeshIndex].m_AniHelper = std::move(helper);
		}
		break;	
	}
}

void AseParser::InputHelper(size_t HelperIndex, HelperType helperType)
{
	auto& helper = m_aseMesh->m_HelperList[HelperIndex];

	std::string buffer;
	std::istringstream is(m_Stream[m_Index]);

	switch (helperType)
	{
	case HelperType::NODE_NAME:
	{
		is >> buffer;
		std::getline(is, buffer);
		std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
		helper.Name = name;
	}break;
	case HelperType::NODE_PARENT:
	{
		is >> buffer;
		std::getline(is, buffer);
		std::string name(buffer, buffer.find_first_of('"') + 1, buffer.find_last_of('"') - 2);
		helper.ParentName = name;
	}break;
	case HelperType::HELPER_CLASS:
	{
		is >> buffer;
		std::getline(is, buffer);
		int findType0 = (int)buffer.find("bone");
		if (findType0 >= 0)
		{
			helper.m_ObjType = ObjectType::BONE;
		}
		else
		{
			helper.m_ObjType = ObjectType::DUMMY;
		}
	}break;
	case HelperType::NODE_TM:
	{
		InputMatrix(&helper);
	}break;
	case HelperType::BOUNDINGBOX_MIN:
	{
		XMFLOAT3 Min;
		XMFLOAT3 Max;
		is >> buffer >> Min.x >> Min.z >> Min.y;
		is.clear();
		is.str(m_Stream[++m_Index]);

		is >> buffer >> Max.x >> Max.z >> Max.y;

		XMVECTOR min = XMLoadFloat3(&Min);
		XMVECTOR max = XMLoadFloat3(&Max);

		XMVECTOR c = (max + min) * 0.5f;

		BoundingBox box;
		XMStoreFloat3(&box.Center, c);
		XMStoreFloat3(&box.Extents, c - min);
		helper.m_BoundingBox = box;
	}break;
	case HelperType::TM_ANIMATION:
	{
		InputAnimation(&helper);
	}break;
	}
}

void AseParser::InputMatrix(Helper* helper)
{
	std::string buffer;

	MatrixType matType = (MatrixType)0;

	XMFLOAT4X4 world;
	XMStoreFloat4x4(&world, XMMatrixIdentity());

	for (;FindType(m_MatrixType.data(), matType); ++m_Index)
	{
		switch (matType)
		{
			case MatrixType::TM_ROW:
			{
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

				helper->matWorld = world;
				--m_Index;
			}break;
			case MatrixType::TM_POS:
			{
				std::istringstream is(m_Stream[m_Index]);
				is >> buffer >> helper->Position.x >> helper->Position.z >> helper->Position.y;

			}break;
			case MatrixType::TM_ROTAXIS:
			{
				std::istringstream is(m_Stream[m_Index]);
				is >> buffer >> helper->Rotation.x >> helper->Rotation.z >> helper->Rotation.y;

				is.clear();
				is.str(m_Stream[++m_Index]);

				is >> buffer >> helper->Rotation.w;
			}break;
			case MatrixType::TM_SCALE:
			{
				std::istringstream is(m_Stream[m_Index]);
				is >> buffer >> helper->Scale.x >> helper->Scale.z >> helper->Scale.y;

				is.clear();
				is.str(m_Stream[++m_Index]);

				is >> buffer >> helper->QuatScale.x >> helper->QuatScale.z >> helper->QuatScale.y;

				is.clear();
				is.str(m_Stream[++m_Index]);

				is >> buffer >> helper->QuatScale.w;
			}break;
		}
	}
}

void AseParser::InputAnimation(Helper * helper)
{
	AnimationType aniType = (AnimationType)0;

	for (;FindType(m_AnimationType.data(), aniType); ++m_Index)
	{
		switch (aniType)
		{
			case AnimationType::CONTROL_POS:
			{
				InputPosTrack(helper);
			}break;
			case AnimationType::CONTROL_ROT:
			{
				InputRotTrack(helper);
			}break;
			case AnimationType::CONTROL_SCALE:
			{
				InputScaleTrack(helper);
			}break;
		}
	}
}

void AseParser::InputPosTrack(Helper * helper)
{
	++m_Index;
	std::string buffer;
	int findType = -1;

	for (;findType < 0; findType = (int)m_Stream[++m_Index].find('}'))
	{
		std::istringstream is(m_Stream[m_Index]);
		AniTrack posTrack;
		is >> buffer >> posTrack.mTick >> posTrack.mPosition.x >> posTrack.mPosition.z >> posTrack.mPosition.y;
		helper->m_Position.push_back(posTrack);
	}
}

void AseParser::InputRotTrack(Helper * helper)
{
	++m_Index;
	std::string buffer;
	int findType = -1;

	for (;findType < 0; findType = (int)m_Stream[++m_Index].find('}'))
	{
		std::istringstream is(m_Stream[m_Index]);
		AniTrack rotTrack;
		XMFLOAT3 rot;
		float angle = 0.0f;
		is >> buffer >> rotTrack.mTick >> rot.x >> rot.z >> rot.y >> angle;
		XMVECTOR quatRot = XMQuaternionRotationAxis(XMLoadFloat3(&rot), angle);
		XMStoreFloat4(&rotTrack.mQuatRotation, quatRot);
		helper->m_Rotation.push_back(rotTrack);
	}

	for (size_t i = 0; i < helper->m_Rotation.size(); ++i)
	{
		XMFLOAT4 quat = helper->m_Rotation[i].mQuatRotation;
		if (i != 0)
		{
			XMFLOAT4 prevQuat = helper->m_Rotation[i - 1].mQuatRotation;
			XMVECTOR prevVec = XMLoadFloat4(&prevQuat);
			XMVECTOR curVec = XMLoadFloat4(&quat);
			curVec = XMQuaternionMultiply(prevVec, curVec);
			XMStoreFloat4(&quat, curVec);
		}
		helper->m_Rotation[i].mQuatRotation = quat;
	}
}

void AseParser::InputScaleTrack(Helper * helper)
{
	++m_Index;
	std::string buffer;
	int findType = -1;

	for (;findType < 0; findType = (int)m_Stream[++m_Index].find('}'))
	{
		std::istringstream is(m_Stream[m_Index]);
		AniTrack scaleTrack;
		XMFLOAT3 rot;
		float angle = 0.0f;
		is >> buffer >> scaleTrack.mTick >> scaleTrack.mPosition.x >> scaleTrack.mPosition.z >> scaleTrack.mPosition.y >> rot.x >> rot.z >> rot.y >> angle;
		if (XMVector3Equal(XMVectorZero(), XMLoadFloat3(&rot)))
		{
			XMStoreFloat4(&scaleTrack.mQuatRotation, XMVectorZero());
		}
		else
		{
			XMVECTOR quatRot = XMQuaternionRotationAxis(XMLoadFloat3(&rot), angle);
			XMStoreFloat4(&scaleTrack.mQuatRotation, quatRot);
		}
		helper->m_Scale.push_back(scaleTrack);
	}
}

void AseParser::InputVertexData(size_t GeomeshIndex)
{
	static const std::array<std::string, 5> vertexType = 
	{ 
		"*MESH_NUMVERTEX",
		"*MESH_NUMTVERTEX",
		"*MESH_NUMCVERTEX",
		"*MESH_NORMALS",
	};

 	auto& ObjList = m_aseMesh->m_ObjectList[GeomeshIndex];
	VertexType vType = (VertexType)0;


	for (;FindType(vertexType.data(), vType); ++m_Index)
	{
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

		std::getline(is, ignore);

		std::string Mtrl(ignore, ignore.find_last_of('D') + 2, ignore.length());

		index.Mtrl = std::stoi(Mtrl);
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
		--m_Index;
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
	--m_Index;
}

void AseParser::Findstring(const std::string & text)
{
	int findText = (int)m_Stream[m_Index].find(text);
	while (findText < 0)
	{
		findText = (int)m_Stream[++m_Index].find(text);
	}
}