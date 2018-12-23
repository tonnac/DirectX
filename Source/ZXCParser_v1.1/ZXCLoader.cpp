#include "ZXCLoader.h"
#include "DxState.h"

using namespace DirectX;

bool ZXCLoader::LoadZXC(const std::wstring & FileName, Mesh * mesh)
{
	mMesh = mesh;
	std::wifstream fp(FileName.c_str());
	std::wstring ignore;

	UINT numMaterials;
	UINT numHelpers;
	UINT numMeshes;
	UINT numVertices;
	UINT numTriangles;
	UINT numAnimationClips;

	if (!fp.is_open())
	{
		std::wstring message = FileName + L"Not Found.";
		MessageBox(nullptr, message.c_str(), 0, 0);
		return false;
	}

	std::getline(fp, ignore);
	std::getline(fp, ignore);
	std::getline(fp, ignore);

	fp >> ignore >> numMaterials;
	fp >> ignore >> numHelpers;
	fp >> ignore >> numMeshes;
	fp >> ignore >> numVertices;
	fp >> ignore >> numTriangles;
	fp >> ignore >> numAnimationClips;

	mesh->m_NodeList.resize(numMeshes + numHelpers);
	for (UINT i = 0; i < numMeshes + numHelpers; ++i)
	{
		mesh->m_NodeList[i] = std::make_unique<Mesh>();
	}
	mesh->m_VertexList.resize(numVertices);
	mesh->m_IndexList.resize(numTriangles * 3);

	LoadScene(fp);
	LoadMaterial(fp, numMaterials);
	LoadMesh(fp, numMeshes);
	LoadHelper(fp, numHelpers);
	ReadVertex(fp, numVertices);
	ReadIndex(fp, numTriangles);

	for (auto & x : mSubsetID)
	{
		for (auto &k : x.second)
		{
			std::unique_ptr<RenderItem> ritem = std::make_unique<RenderItem>();
			ritem->Subgeo.IndexCount;
			ritem->Subgeo.IndexCount;
			ritem->Subgeo.IndexCount;
		}
		
	}


	return true;
}

void ZXCLoader::LoadScene(std::wifstream& fp)
{
	std::wstring ignore;

	UINT firstFrame = 0;
	UINT lastFrame = 0;
	UINT frameSpeed = 0;
	UINT frameTick = 0;

	fp >> ignore;
	fp >> ignore >> firstFrame;
	fp >> ignore >> lastFrame;
	fp >> ignore >> frameSpeed;
	fp >> ignore >> frameTick;
}

void ZXCLoader::LoadMaterial(std::wifstream& fp, UINT numMaterials)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < numMaterials; ++i)
	{
		UINT rootMapNum;
		UINT subMtlNum;
		fp >> ignore >> ignore;
		fp >> ignore >> rootMapNum;
		fp >> ignore >> subMtlNum;

		for (UINT j = 0; j < subMtlNum; ++j)
		{
			UINT subMapNum;
			UINT subNo;
			std::wstring fileName;

			fp >> ignore >> ignore;
			fp >> ignore >> subMapNum;

			for (UINT k = 0; k < subMapNum; ++k)
			{
				fp >> ignore >> subNo;
				fp >> ignore >> ignore;
				mMaterials[std::pair<UINT, UINT>(i, j)].push_back(std::pair<UINT, std::wstring>(subNo, ignore));
			}
		}
		for (UINT j = 0; j < rootMapNum; ++j)
		{
			UINT subMapNum;
			UINT subNo;
			std::wstring fileName;

			fp >> ignore >> ignore;
			fp >> ignore >> subMapNum;

			for (UINT k = 0; k < subMapNum; ++k)
			{
				fp >> ignore >> subNo;
				fp >> ignore >> ignore;
				mMaterials[std::pair<UINT, UINT>(i, j)].push_back(std::pair<UINT, std::wstring>(subNo, ignore));
			}
		}
	}
}

void ZXCLoader::LoadMesh(std::wifstream& fp, UINT numMeshes)
{
	std::wstring ignore;

	fp >> ignore;
	for (UINT i = 0; i < numMeshes; ++i)
	{
		UINT Index;
		UINT ParentNum;
		fp >> ignore >> ignore >> Index;
		fp >> ignore >> ignore >> ParentNum;
		fp >> ignore >> ignore;
		fp >> ignore;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._11 >> mMesh->m_NodeList[i]->m_matWorld._12 >> mMesh->m_NodeList[i]->m_matWorld._13 >> mMesh->m_NodeList[i]->m_matWorld._14;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._21 >> mMesh->m_NodeList[i]->m_matWorld._22 >> mMesh->m_NodeList[i]->m_matWorld._23 >> mMesh->m_NodeList[i]->m_matWorld._24;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._31 >> mMesh->m_NodeList[i]->m_matWorld._32 >> mMesh->m_NodeList[i]->m_matWorld._33 >> mMesh->m_NodeList[i]->m_matWorld._34;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._41 >> mMesh->m_NodeList[i]->m_matWorld._42 >> mMesh->m_NodeList[i]->m_matWorld._43 >> mMesh->m_NodeList[i]->m_matWorld._44;
		fp >> ignore >> mMesh->m_NodeList[i]->pos.x >> mMesh->m_NodeList[i]->pos.y >> mMesh->m_NodeList[i]->pos.z;
		fp >> ignore >> mMesh->m_NodeList[i]->rot.x >> mMesh->m_NodeList[i]->rot.y >> mMesh->m_NodeList[i]->rot.z >> mMesh->m_NodeList[i]->rot.w;
		fp >> ignore >> mMesh->m_NodeList[i]->scaleaxis.x >> mMesh->m_NodeList[i]->scaleaxis.y >> mMesh->m_NodeList[i]->scaleaxis.z >> mMesh->m_NodeList[i]->scaleaxis.z;
		fp >> ignore >> mMesh->m_NodeList[i]->scale.x >> mMesh->m_NodeList[i]->scale.y >> mMesh->m_NodeList[i]->scale.z;
	}
}

void ZXCLoader::LoadHelper(std::wifstream& fp, UINT numHelpers)
{
	std::wstring ignore;

	fp >> ignore;
	for (UINT i = 0; i < numHelpers; ++i)
	{
		UINT Index;
		UINT ParentNum;
		fp >> ignore >> ignore >> Index;
		fp >> ignore >> ignore >> ParentNum;
		fp >> ignore >> ignore;
		fp >> ignore;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._11 >> mMesh->m_NodeList[i]->m_matWorld._12 >> mMesh->m_NodeList[i]->m_matWorld._13 >> mMesh->m_NodeList[i]->m_matWorld._14;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._21 >> mMesh->m_NodeList[i]->m_matWorld._22 >> mMesh->m_NodeList[i]->m_matWorld._23 >> mMesh->m_NodeList[i]->m_matWorld._24;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._31 >> mMesh->m_NodeList[i]->m_matWorld._32 >> mMesh->m_NodeList[i]->m_matWorld._33 >> mMesh->m_NodeList[i]->m_matWorld._34;
		fp >> ignore >> mMesh->m_NodeList[i]->m_matWorld._41 >> mMesh->m_NodeList[i]->m_matWorld._42 >> mMesh->m_NodeList[i]->m_matWorld._43 >> mMesh->m_NodeList[i]->m_matWorld._44;
		fp >> ignore >> mMesh->m_NodeList[i]->pos.x >> mMesh->m_NodeList[i]->pos.y >> mMesh->m_NodeList[i]->pos.z;
		fp >> ignore >> mMesh->m_NodeList[i]->rot.x >> mMesh->m_NodeList[i]->rot.y >> mMesh->m_NodeList[i]->rot.z >> mMesh->m_NodeList[i]->rot.w;
		fp >> ignore >> mMesh->m_NodeList[i]->scaleaxis.x >> mMesh->m_NodeList[i]->scaleaxis.y >> mMesh->m_NodeList[i]->scaleaxis.z >> mMesh->m_NodeList[i]->scaleaxis.z;
		fp >> ignore >> mMesh->m_NodeList[i]->scale.x >> mMesh->m_NodeList[i]->scale.y >> mMesh->m_NodeList[i]->scale.z;
		fp >> ignore >> ignore >> ignore >> ignore;
		fp >> ignore >> ignore >> ignore >> ignore;
	}
}

void ZXCLoader::ReadVertex(std::wifstream & fp, UINT numVertices)
{
	std::wstring ignore;
	fp >> ignore;

	for (UINT i = 0; i < numVertices; ++i)
	{
		fp >> ignore >> mMesh->m_VertexList[i].p.x >> mMesh->m_VertexList[i].p.y >> mMesh->m_VertexList[i].p.z;
		fp >> ignore >> mMesh->m_VertexList[i].n.x >> mMesh->m_VertexList[i].n.y >> mMesh->m_VertexList[i].n.z;
		fp >> ignore >> mMesh->m_VertexList[i].c.x >> mMesh->m_VertexList[i].c.y >> mMesh->m_VertexList[i].c.z >> mMesh->m_VertexList[i].c.w;
		fp >> ignore >> mMesh->m_VertexList[i].t.x >> mMesh->m_VertexList[i].t.y;
	}
}

void ZXCLoader::ReadIndex(std::wifstream & fp, UINT numIndices)
{
	std::wstring ignore;
	fp >> ignore;
	for (UINT i = 0; i < numIndices; ++i)
	{
		fp >> ignore >> mMesh->m_IndexList[i * 3 + 0] >> mMesh->m_IndexList[i * 3 + 1] >> mMesh->m_IndexList[i * 3 + 2];
	}
}

void ZXCLoader::InputMatrix(DirectX::XMFLOAT4X4 & m, size_t& index)
{
}

//void ZXCLoader::InputAnimation(Helper* h, size_t& index, int pani, int rani, int sani)
//{
//}

