#include "Mesh.h"
#include <stack>

std::unique_ptr<Mesh> ZXCConverter::Convert(ZXCMesh * zxcMesh, ID3D11Device* device)
{
	std::unique_ptr<Mesh> rMesh = std::make_unique<Mesh>();

//	rMesh->m_Objectlist.resize(zxcMesh->m_ObjectList.size() + zxcMesh->m_HelperList.size());
//	rMesh->m_iNumFaces = 0;
//
////	rMesh->m_iNumFaces += (int)zxcMesh->m_ObjectList[i].posFaceList.size();
//
//	for (int i = 0; i < (int)zxcMesh->m_ObjectList.size(); ++i)
//	{
//		std::unique_ptr<Mesh> subMesh = std::make_unique<Mesh>();
//		subMesh->m_Objectlist.resize(zxcMesh->m_MateriaList[i].SubMaterial.size());
//		for (int i = 0; i < (int)subMesh->m_Objectlist.size(); ++i)
//		{
//			subMesh->m_Objectlist[i] = std::make_unique<Mesh>();
//		}
//		subMesh->m_iNumFaces = (int)zxcMesh->m_ObjectList[i].posFaceList.size();
//		for (int iFace = 0; iFace < subMesh->m_iNumFaces; ++iFace)
//		{
//			int MtrlIndex = zxcMesh->m_ObjectList[i].posFaceList[iFace].Mtrl;
//			auto& mesh = subMesh->m_Objectlist[MtrlIndex];
//			for (int iVer = 0; iVer < 3; ++iVer)
//			{
//				int vID = iFace * 3 + iVer;
//				PNCT_VERTEX vertex;
//				//pos
//				if (zxcMesh->m_ObjectList[i].vertexList.size() > 0)
//				{
//					int index = zxcMesh->m_ObjectList[i].posFaceList[iFace].v[iVer];
//					vertex.p = (float*)&zxcMesh->m_ObjectList[i].vertexList[index];
//				}
//
//				//normal
//				if (zxcMesh->m_ObjectList[i].norList.size() > 0)
//				{
//					vertex.n = (float*)&zxcMesh->m_ObjectList[i].norList[vID];
//				}
//
//				//color
//				if (zxcMesh->m_ObjectList[i].colorFaceList.size() > 0)
//				{
//					int index = zxcMesh->m_ObjectList[i].colorFaceList[iFace].v[iVer];
//					vertex.c = (float*)&zxcMesh->m_ObjectList[i].colorList[index];
//					vertex.c.w = 1.0f;
//				}
//
//				//tex
//				if (zxcMesh->m_ObjectList[i].texFaceList.size() > 0)
//				{
//					int index = zxcMesh->m_ObjectList[i].texFaceList[iFace].v[iVer];
//					vertex.t = (float*)&zxcMesh->m_ObjectList[i].texList[index];
//				}
//				mesh->m_VertexList.push_back(std::move(vertex));
//			}
//		}
//
//		for (size_t k = 0; k < subMesh->m_Objectlist.size(); ++k)
//		{
//			auto& obj = subMesh->m_Objectlist[k];
//			std::tstring texName = zxcMesh->m_MateriaList[i].SubMaterial[k].Texture[0].Filename;
//			std::tstring texPath = L"..\\..\\data\\tex\\";
//			obj->m_DxObject.m_iNumVertex = (UINT)obj->m_VertexList.size();
//			obj->m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
//			obj->Create(device, L"shape.hlsl", texPath + texName);
//		}
//
//		rMesh->m_Objectlist[i] = std::move(subMesh);
//	}
	return std::move(rMesh);
}

std::unique_ptr<Mesh> ZXCConverter::Convert0(ZXCMesh * zxcMesh, ID3D11Device * device)
{
	std::unordered_map<std::string, std::unique_ptr<Mesh>> rMesh;

	for (size_t i = 0; i < zxcMesh->m_HelperList.size(); ++i)
	{
		auto& helper = zxcMesh->m_HelperList[i];
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();

		mesh->m_Scene = zxcMesh->m_Scene;

		mesh->m_Position.insert(mesh->m_Position.end(), helper.m_Position.begin(), helper.m_Position.end());
		mesh->m_Rotation.insert(mesh->m_Rotation.end(), helper.m_Rotation.begin(), helper.m_Rotation.end());
		mesh->m_Scale.insert(mesh->m_Scale.end(), helper.m_Scale.begin(), helper.m_Scale.end());
		mesh->Name = helper.Name;
		mesh->ParentName = helper.ParentName;
		mesh->m_Type = helper.m_ObjType;
		mesh->m_BoundingBox = helper.m_BoundingBox;
		mesh->m_matWorld = (float*)&helper.matWorld;

		D3DXMatrixInverse(&mesh->InvWorld, nullptr, &mesh->m_matWorld);

		CopyMemory(&mesh->Rotation.m[0], &helper.Rotation, sizeof(float) * 4);
		CopyMemory(&mesh->Scale.m[0], &helper.Scale, sizeof(float) * 3);
		CopyMemory(&mesh->Scale.m[1], &helper.QuatScale, sizeof(float) * 4);
		CopyMemory(&mesh->Translation.m[0], &helper.Position, sizeof(float) * 3);

		rMesh.insert(std::make_pair(mesh->Name, std::move(mesh)));
	}

	for (size_t i = 0; i < zxcMesh->m_ObjectList.size(); ++i)
	{
		auto& geoObj = zxcMesh->m_ObjectList[i];
		std::unique_ptr<Mesh> mesh = std::make_unique<Mesh>();
		int mtrlRef = geoObj.mtrlRef;

		if (geoObj.m_AniHelper != nullptr)
		{
			mesh->m_Position.insert(mesh->m_Position.end(), geoObj.m_AniHelper->m_Position.begin(), geoObj.m_AniHelper->m_Position.end());
			mesh->m_Rotation.insert(mesh->m_Rotation.end(), geoObj.m_AniHelper->m_Rotation.begin(), geoObj.m_AniHelper->m_Rotation.end());
			mesh->m_Scale.insert(mesh->m_Scale.end(), geoObj.m_AniHelper->m_Scale.begin(), geoObj.m_AniHelper->m_Scale.end());
		}
		mesh->Name = std::string(geoObj.Name.begin(), geoObj.Name.end());
		mesh->ParentName = geoObj.ParentName;
		mesh->m_Type = ObjectType::GEOMESH;
		mesh->m_matWorld = (float*)&geoObj.m_Helper.matWorld;

		D3DXMatrixInverse(&mesh->InvWorld, nullptr, &mesh->m_matWorld);

		CopyMemory(&mesh->Rotation.m[0], &geoObj.m_Helper.Rotation, sizeof(float) * 4);
		CopyMemory(&mesh->Scale.m[0], &geoObj.m_Helper.Scale, sizeof(float) * 3);
		CopyMemory(&mesh->Scale.m[1], &geoObj.m_Helper.QuatScale, sizeof(float) * 4);
		CopyMemory(&mesh->Translation.m[0], &geoObj.m_Helper.Position, sizeof(float) * 3);

		mesh->m_ObjectList.resize(geoObj.mSubMesh.size());

		if (!geoObj.mSubMesh.empty())
		{
			for (int k = 0; k < (int)geoObj.mSubMesh.size(); ++k)
			{
				auto & subMesh = mesh->m_ObjectList[k];

				subMesh = std::make_unique<Mesh>();
				mesh->m_Scene = zxcMesh->m_Scene;
				mesh->m_iNumFaces = (int)geoObj.mSubMesh[k]->indices.size() / 3;
				subMesh->m_VertexList.insert(subMesh->m_VertexList.end(), geoObj.mSubMesh[k]->vertices.begin(), geoObj.mSubMesh[k]->vertices.end());
				subMesh->m_IndexList.insert(subMesh->m_IndexList.end(), geoObj.mSubMesh[k]->indices.begin(), geoObj.mSubMesh[k]->indices.end());

				std::tstring texName = zxcMesh->m_MateriaList[mtrlRef].SubMaterial[k].Texture[0].Filename;
				std::tstring texPath = L"..\\..\\data\\maps\\";

				subMesh->m_DxObject.m_iNumIndex = (UINT)subMesh->m_IndexList.size();
				subMesh->m_DxObject.m_iNumVertex = (UINT)subMesh->m_VertexList.size();
				subMesh->m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
				subMesh->Create(device, L"shape.hlsl", texPath + texName);
			}
		}
		else
		{
			mesh->m_Scene = zxcMesh->m_Scene;
			mesh->m_iNumFaces = (int)geoObj.indices.size() / 3;
			mesh->m_VertexList.insert(mesh->m_VertexList.end(), geoObj.vertices.begin(), geoObj.vertices.end());
			mesh->m_IndexList.insert(mesh->m_IndexList.end(), geoObj.indices.begin(), geoObj.indices.end());

			std::tstring texName;
			if (geoObj.mtlID == -1)
				texName = zxcMesh->m_MateriaList[mtrlRef].Texture[0].Filename;
			else
			{
				texName = zxcMesh->m_MateriaList[mtrlRef].SubMaterial[geoObj.mtlID].Texture[0].Filename;
			}
			std::tstring texPath = L"..\\..\\data\\maps\\";

			mesh->m_DxObject.m_iNumIndex = (UINT)mesh->m_IndexList.size();
			mesh->m_DxObject.m_iNumVertex = (UINT)mesh->m_VertexList.size();
			mesh->m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
			mesh->Create(device, L"shape.hlsl", texPath + texName);
		}

		rMesh.insert(std::make_pair(mesh->Name, std::move(mesh)));
	}

	for (auto&x : rMesh)
	{
		auto& mesh = x.second;
		if (!mesh->ParentName.empty())
		{
			mesh->m_Parent = rMesh[mesh->ParentName].get();
			rMesh[mesh->ParentName]->m_ChildList.push_back(mesh.get());
		}
	}

	std::string RootName;
	for (auto &x : rMesh)
	{
		if (x.second->m_Parent == nullptr)
		{
			RootName = x.second->Name;
		}
	}

	for (auto &x : rMesh)
	{
		if (x.second->Name != RootName)
		{
			if (x.second->m_Type == ObjectType::GEOMESH)
			{
				rMesh[RootName]->m_RenderList.push_back(x.second.get());
			}
			rMesh[RootName]->m_NodeList.push_back(std::move(x.second));
		}
	}

	rMesh[RootName]->Init();

	return std::move(rMesh[RootName]);

}
