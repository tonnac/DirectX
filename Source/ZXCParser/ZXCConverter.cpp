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
		std::unique_ptr<Mesh> submesh = std::make_unique<Mesh>();

		submesh->m_Scene = zxcMesh->m_Scene;

		submesh->m_Position.insert(submesh->m_Position.end(), helper.m_Position.begin(), helper.m_Position.end());
		submesh->m_Rotation.insert(submesh->m_Rotation.end(), helper.m_Rotation.begin(), helper.m_Rotation.end());
		submesh->m_Scale.insert(submesh->m_Scale.end(), helper.m_Scale.begin(), helper.m_Scale.end());
		submesh->Name = helper.Name;
		submesh->ParentName = helper.ParentName;
		submesh->m_Type = helper.m_ObjType;
		submesh->m_BoundingBox = helper.m_BoundingBox;
		submesh->m_matWorld = (float*)&helper.matWorld;

		D3DXMatrixInverse(&submesh->InvWorld, nullptr, &submesh->m_matWorld);

		CopyMemory(&submesh->Rotation.m[0], &helper.Rotation, sizeof(float) * 4);
		CopyMemory(&submesh->Scale.m[0], &helper.Scale, sizeof(float) * 3);
		CopyMemory(&submesh->Scale.m[1], &helper.QuatScale, sizeof(float) * 4);
		CopyMemory(&submesh->Translation.m[0], &helper.Position, sizeof(float) * 3);

		rMesh.insert(std::make_pair(submesh->Name, std::move(submesh)));
	}

	for (size_t i = 0; i < zxcMesh->m_ObjectList.size(); ++i)
	{
		auto& geoObj = zxcMesh->m_ObjectList[i];
		std::unique_ptr<Mesh> subMesh = std::make_unique<Mesh>();
		int mtrlRef = geoObj.mtrlRef;

		if (geoObj.m_AniHelper != nullptr)
		{
			subMesh->m_Position.insert(subMesh->m_Position.end(), geoObj.m_AniHelper->m_Position.begin(), geoObj.m_AniHelper->m_Position.end());
			subMesh->m_Rotation.insert(subMesh->m_Rotation.end(), geoObj.m_AniHelper->m_Rotation.begin(), geoObj.m_AniHelper->m_Rotation.end());
			subMesh->m_Scale.insert(subMesh->m_Scale.end(), geoObj.m_AniHelper->m_Scale.begin(), geoObj.m_AniHelper->m_Scale.end());
		}
		subMesh->Name = std::string(geoObj.Name.begin(), geoObj.Name.end());
		subMesh->ParentName = geoObj.ParentName;
		subMesh->m_Type = ObjectType::GEOMESH;
		subMesh->m_matWorld = (float*)&geoObj.m_Helper.matWorld;

		D3DXMatrixInverse(&subMesh->InvWorld, nullptr, &subMesh->m_matWorld);

		CopyMemory(&subMesh->Rotation.m[0], &geoObj.m_Helper.Rotation, sizeof(float) * 4);
		CopyMemory(&subMesh->Scale.m[0], &geoObj.m_Helper.Scale, sizeof(float) * 3);
		CopyMemory(&subMesh->Scale.m[1], &geoObj.m_Helper.QuatScale, sizeof(float) * 4);
		CopyMemory(&subMesh->Translation.m[0], &geoObj.m_Helper.Position, sizeof(float) * 3);


		if(!zxcMesh->m_MateriaList[mtrlRef].SubMaterial.empty())
			subMesh->m_ObjectList.resize(zxcMesh->m_MateriaList[mtrlRef].SubMaterial.size());
		else
		{
			subMesh->m_ObjectList.resize(zxcMesh->m_MateriaList.size());
		}
		for (int k = 0; k < (int)subMesh->m_ObjectList.size(); ++k)
		{
			subMesh->m_ObjectList[k] = std::make_unique<Mesh>();
			subMesh->m_Scene = zxcMesh->m_Scene;
		}

		if (zxcMesh->m_MateriaList[mtrlRef].SubMaterial.size() != 0)
			subMesh->m_ObjectList.resize(zxcMesh->m_MateriaList[mtrlRef].SubMaterial.size());
		else
		{
			subMesh->m_ObjectList.resize(zxcMesh->m_MateriaList.size());
		}
		for (int k = 0; k < (int)subMesh->m_ObjectList.size(); ++k)
		{
			subMesh->m_ObjectList[k] = std::make_unique<Mesh>();
			subMesh->m_Scene = zxcMesh->m_Scene;
			subMesh->m_iNumFaces = (int)zxcMesh->m_ObjectList[i].indices.size() / 3;
			subMesh->m_ObjectList[k]->m_VertexList.insert(subMesh->m_ObjectList[k]->m_VertexList.end(), zxcMesh->m_ObjectList[i].vertices.begin(), zxcMesh->m_ObjectList[i].vertices.end());
			subMesh->m_ObjectList[k]->m_IndexList.insert(subMesh->m_ObjectList[k]->m_IndexList.end(), zxcMesh->m_ObjectList[i].indices.begin(), zxcMesh->m_ObjectList[i].indices.end());
		}

		for (size_t k = 0; k < subMesh->m_ObjectList.size(); ++k)
		{
			auto& obj = subMesh->m_ObjectList[k];
			std::tstring texName;
			std::tstring texPath = L"..\\..\\data\\";
			if (zxcMesh->m_MateriaList[mtrlRef].SubMaterial.size() != 0)
			{
				if(!zxcMesh->m_MateriaList[mtrlRef].SubMaterial[k].Texture.empty())
					texName = zxcMesh->m_MateriaList[mtrlRef].SubMaterial[k].Texture[0].Filename;
				else
				{
					texName = std::tstring();
				}
			}
			else if(zxcMesh->m_MateriaList.size() != 0)
			{
				texName = zxcMesh->m_MateriaList[mtrlRef].Texture[0].Filename;
			}
			else
			{
				texName = std::tstring();
				texPath = std::tstring();
			}
			obj->m_DxObject.m_iNumIndex = (UINT)obj->m_IndexList.size();
			obj->m_DxObject.m_iNumVertex = (UINT)obj->m_VertexList.size();
			obj->m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
			if(obj->m_DxObject.m_iNumVertex > 0)
				obj->Create(device, L"shape.hlsl", texPath + texName);
		}

		std::vector<std::unique_ptr<Mesh>>::iterator iter = subMesh->m_ObjectList.begin();
		for (;iter != subMesh->m_ObjectList.end();)
		{
			if ((*iter)->m_VertexList.empty())
			{
				iter = subMesh->m_ObjectList.erase(iter);
			}
			else
			{
				++iter;
			}
		}

		rMesh.insert(std::make_pair(subMesh->Name, std::move(subMesh)));
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

	std::unordered_map<std::string, std::unique_ptr<Mesh>>::const_iterator iter = rMesh.begin();
	std::vector<Mesh*>::const_iterator childiter;

	for (;iter != rMesh.end();)
	{
		if (iter->second->m_ChildList.size() == 0 && iter->second->m_Type < ObjectType::GEOMESH)
		{
			if (iter->second->m_Parent != nullptr)
			{
				childiter = iter->second->m_Parent->m_ChildList.begin();
				for (;childiter != iter->second->m_Parent->m_ChildList.end(); ++childiter)
				{
					if ((*childiter)->Name == iter->second->Name)
					{
						childiter = iter->second->m_Parent->m_ChildList.erase(childiter);
						break;
					}
				}
			}
			iter = rMesh.erase(iter);
		}
		else
		{
			++iter;
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
