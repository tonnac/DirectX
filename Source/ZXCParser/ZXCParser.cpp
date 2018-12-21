#include "ZXCParser.h"
#include "ZXCMesh.h"
#include "DxState.h"

using namespace DirectX;

const std::array<std::wstring, 4> ZXCParser::m_Type =
{
	L"#SCENEINFO",
	L"#MATERIAL_NUM",
	L"#GEOMMESH_NUM",
	L"#HELPER_NUM"
};

bool ZXCParser::LoadZXC(const std::wstring & FileName, ZXCMesh * mesh)
{
	m_zxcMesh = mesh;

	std::wifstream fp(FileName.c_str());
	std::wstring buffer;

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
				m_GeomeshIndex = m_Stream.size();
				break;
			case 3:
				m_HelperObjIndex = m_Stream.size();
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
	LoadHelperObject();

	return true;
}

void ZXCParser::LoadScene()
{
	size_t index = m_ScnenIndex + 1;
	std::wistringstream is(m_Stream[index]);
	std::wstring ignore;
	sScene scene;
	
	is >> ignore >> scene.FirstFrame;

	is.clear();
	is.str(m_Stream[++index]);

	is >> ignore >> scene.LastFrame;
	
	is.clear();
	is.str(m_Stream[++index]);

	is >> ignore >> scene.FrameSpeed;

	is.clear();
	is.str(m_Stream[++index]);

	is >> ignore >> scene.TicksperFrame;

	m_zxcMesh->m_Scene = scene;
}

void ZXCParser::LoadMaterial()
{
	size_t index = m_MaterialIndex;

	std::wistringstream is(m_Stream[index]);
	std::wstring ignore;
	int materialNum;

	is >> ignore >> materialNum;

	m_zxcMesh->m_MateriaList.resize(materialNum);

	for (int i = 0; i < materialNum; ++i)
	{
		int texSize;
		int subMtrlSize;
		auto& m = m_zxcMesh->m_MateriaList[i];

		is.clear();
		is.str(m_Stream[++index]);
		is >> ignore >> ignore >> ignore >> m.Name >> ignore >> texSize >> ignore >> subMtrlSize;

		m.SubMaterial.resize(subMtrlSize);
		for (int j = 0; j < subMtrlSize; ++j)
		{
			int subTexSize;
			auto& msub = m.SubMaterial[j];
			is.clear();
			is.str(m_Stream[++index]);
			is >> ignore >> ignore >> ignore >> msub.Name >> ignore >> subTexSize;

			msub.Texture.resize(subTexSize);
			for (int p = 0; p < subTexSize; ++p)
			{
				auto& msubTex = msub.Texture[p];
				is.clear();
				is.str(m_Stream[++index]);
				is >> ignore >> msubTex.SubNo >> ignore >> msubTex.Filename;
			}

		}

		m.Texture.resize(texSize);
		for (int k = 0; k < texSize; ++k)
		{
			auto& tex = m.Texture[k];
			is.clear();
			is.str(m_Stream[++index]);
			is >> ignore >> tex.SubNo >> ignore >> tex.Filename;
		}

	}
}

void ZXCParser::LoadGeomesh()
{
	size_t index = m_GeomeshIndex;

	std::wistringstream is(m_Stream[index]);
	std::wstring ignore;
	int geoMeshNum;

	is >> ignore >> geoMeshNum;

	m_zxcMesh->m_ObjectList.resize(geoMeshNum);

	for (int i = 0; i < geoMeshNum; ++i)
	{
		int pani, rani, sani;
		auto& geo = m_zxcMesh->m_ObjectList[i];
		std::wstring parent;
		is.clear();
		is.str(m_Stream[++index]);

		is >> ignore >> ignore >> ignore >> geo.Name >> ignore >> parent >>
			ignore >> geo.mtrlRef >> ignore >> pani >> ignore >> rani >> ignore >> sani;

		if (parent != L"NONE")
			geo.ParentName = std::string(parent.begin(), parent.end());

		++index;
		is.clear();
		is.str(m_Stream[++index]);
		is >> geo.m_Helper.matWorld._11 >> geo.m_Helper.matWorld._12 >> geo.m_Helper.matWorld._13 >> geo.m_Helper.matWorld._14;

		is.clear();
		is.str(m_Stream[++index]);
		is >> geo.m_Helper.matWorld._21 >> geo.m_Helper.matWorld._22 >> geo.m_Helper.matWorld._23 >> geo.m_Helper.matWorld._24;

		is.clear();
		is.str(m_Stream[++index]);
		is >> geo.m_Helper.matWorld._31 >> geo.m_Helper.matWorld._32 >> geo.m_Helper.matWorld._33 >> geo.m_Helper.matWorld._34;

		is.clear();
		is.str(m_Stream[++index]);
		is >> geo.m_Helper.matWorld._41 >> geo.m_Helper.matWorld._42 >> geo.m_Helper.matWorld._43 >> geo.m_Helper.matWorld._44;

		is.clear();
		is.str(m_Stream[++index]);
		int mtlNum;
		is >> ignore >> mtlNum;

		if (mtlNum > 1)
		{
			geo.mSubMesh.resize(mtlNum);
			for (int k = 0; k < mtlNum; ++k)
			{
				geo.mSubMesh[k] = std::make_unique<GeomMesh>();
				auto& subGeo = geo.mSubMesh[k];

				is.clear();
				is.str(m_Stream[++index]);
				is >> ignore >> subGeo->mtlID;

				is.clear();
				is.str(m_Stream[++index]);
				int vertexNum;

				is >> ignore >> vertexNum;

				subGeo->vertices.resize(vertexNum);

				for (int j = 0; j < vertexNum; ++j)
				{
					auto& v = subGeo->vertices[j];

					is.clear();
					is.str(m_Stream[++index]);
					is >> ignore >> v.p.x >> v.p.y >> v.p.z >> ignore >> v.n.x >> v.n.y >> v.n.z >> ignore >> v.c.x >> v.c.y >> v.c.z >> v.c.w >> ignore >> v.t.x >> v.t.y;
				}
				is.clear();
				is.str(m_Stream[++index]);
				int indicesNum;

				is >> ignore >> indicesNum;

				subGeo->indices.resize(indicesNum * 3);

				for (int j = 0; j < indicesNum; ++j)
				{
					auto& i = subGeo->indices;

					std::uint32_t i0 = j * 3 + 0;
					std::uint32_t i1 = j * 3 + 1;
					std::uint32_t i2 = j * 3 + 2;
					is.clear();
					is.str(m_Stream[++index]);

					is >> i[i0] >> i[i1] >> i[i2];
				}
			}
		}
		else
		{
			is.clear();
			is.str(m_Stream[++index]);
			is >> ignore >> geo.mtlID;

			is.clear();
			is.str(m_Stream[++index]);
			int vertexNum;

			is >> ignore >> vertexNum;

			geo.vertices.resize(vertexNum);

			for (int j = 0; j < vertexNum; ++j)
			{
				auto& v = geo.vertices[j];

				is.clear();
				is.str(m_Stream[++index]);
				is >> ignore >> v.p.x >> v.p.y >> v.p.z >> ignore >> v.n.x >> v.n.y >> v.n.z >> ignore >> v.c.x >> v.c.y >> v.c.z >> v.c.w >> ignore >> v.t.x >> v.t.y;
			}
			is.clear();
			is.str(m_Stream[++index]);
			int indicesNum;

			is >> ignore >> indicesNum;

			geo.indices.resize(indicesNum * 3);

			for (int j = 0; j < indicesNum; ++j)
			{
				auto& i = geo.indices;

				std::uint32_t i0 = j * 3 + 0;
				std::uint32_t i1 = j * 3 + 1;
				std::uint32_t i2 = j * 3 + 2;
				is.clear();
				is.str(m_Stream[++index]);

				is >> i[i0] >> i[i1] >> i[i2];
			}
		}

		if (pani > 0)
		{
			geo.m_AniHelper = std::make_unique<Helper>();
			while (m_Stream[++index].find(L"#CONTROL_POS_TRACK") == std::wstring::npos);
			geo.m_AniHelper->m_Position.resize(pani);
			for (int i = 0; i < pani; ++i)
			{
				auto& postrack = geo.m_AniHelper->m_Position[i];

				is.clear();
				is.str(m_Stream[++index]);
				is >> ignore >> postrack.mTick >> postrack.mPosition.x >> postrack.mPosition.y >> postrack.mPosition.z;
			}
		}

		if (rani > 0)
		{
			if(geo.m_AniHelper == nullptr)
				geo.m_AniHelper = std::make_unique<Helper>();
			while (m_Stream[++index].find(L"#CONTROL_ROT_TRACK") == std::wstring::npos);
			geo.m_AniHelper->m_Rotation.resize(rani);
			for (int i = 0; i < rani; ++i)
			{
				auto& rottrack = geo.m_AniHelper->m_Rotation[i];

				is.clear();
				is.str(m_Stream[++index]);
				is >> ignore >> rottrack.mTick >> rottrack.mQuatRotation.x >> rottrack.mQuatRotation.y >> rottrack.mQuatRotation.z >> rottrack.mQuatRotation.w;
			}
		}

		if (sani > 0)
		{
			if (geo.m_AniHelper == nullptr)
				geo.m_AniHelper = std::make_unique<Helper>();
			while (m_Stream[++index].find(L"#CONTROL_SCALE_TRACK") == std::wstring::npos);
			geo.m_AniHelper->m_Scale.resize(sani);
			for (int i = 0; i < sani; ++i)
			{
				auto& scaletrack = geo.m_AniHelper->m_Scale[i];

				is.clear();
				is.str(m_Stream[++index]);
				is >> ignore >> scaletrack.mTick >> scaletrack.mQuatRotation.x >> scaletrack.mQuatRotation.y >> scaletrack.mQuatRotation.z >> scaletrack.mQuatRotation.w >> scaletrack.mPosition.x >> scaletrack.mPosition.y >> scaletrack.mPosition.z;
			}
		}
	}
}

void ZXCParser::LoadHelperObject()
{
}
