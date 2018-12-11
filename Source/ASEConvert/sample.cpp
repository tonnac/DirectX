#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <chrono>
using namespace DirectX;


Sample::Sample()
{}
Sample::~Sample()
{}

int Sample::isEqualVertexList(Mesh& mesh, PNCT_VERTEX& v)
{
	for (int iList = 0; iList < mesh.m_VertexList.size(); ++iList)
	{
		if (v == mesh.m_VertexList[iList])
		{
			return iList;
		}
	}

	return -1;
}

bool Sample::Init()
{
	LARGE_INTEGER fre;
	LARGE_INTEGER bef;
	LARGE_INTEGER bef0;
	LARGE_INTEGER aft;
	QueryPerformanceFrequency(&fre);
	QueryPerformanceCounter(&bef);
	QueryPerformanceCounter(&bef0);

	float time = 0.0f;
	if (m_obj.Load(L"eee.ase"))
	{
		QueryPerformanceCounter(&aft);
		time = (float)(aft.LowPart - bef.LowPart) / (float)fre.LowPart;

		QueryPerformanceCounter(&bef);
		for (size_t i = 0; i < m_obj.m_ObjectList.size(); ++i)
		{
			Mesh mesh;
			m_xObj.m_iNumFaces = m_obj.m_ObjectList[i].posFaceList.size();
			mesh.m_tmpVertexList.resize(m_xObj.m_iNumFaces * 3);
			for (int iFace = 0; iFace < m_xObj.m_iNumFaces; ++iFace)
			{
				for (int iVer = 0; iVer < 3; ++iVer)
				{
					int vID = iFace * 3 + iVer;

					//pos
					if (m_obj.m_ObjectList[i].vertexList.size() > 0)
					{
						int index = m_obj.m_ObjectList[i].posFaceList[iFace].v[iVer];
						mesh.m_tmpVertexList[vID].p = (float*)&m_obj.m_ObjectList[i].vertexList[index];
					}

					//normal
					if (m_obj.m_ObjectList[i].norList.size() > 0)
					{
						mesh.m_tmpVertexList[vID].n = (float*)&m_obj.m_ObjectList[i].norList[vID];
					}

					//color
					if (m_obj.m_ObjectList[i].colorFaceList.size() > 0)
					{
						int index = m_obj.m_ObjectList[i].colorFaceList[iFace].v[iVer];
						mesh.m_tmpVertexList[vID].c = (float*)&m_obj.m_ObjectList[i].colorList[index];
					}

					//tex
					if (m_obj.m_ObjectList[i].texFaceList.size() > 0)
					{
						int index = m_obj.m_ObjectList[i].texFaceList[iFace].v[iVer];
						mesh.m_tmpVertexList[vID].t = (float*)&m_obj.m_ObjectList[i].texList[index];
					}
				}
			}
			QueryPerformanceCounter(&aft);
			time = (float)(aft.LowPart - bef.LowPart) / (float)fre.LowPart;

			QueryPerformanceCounter(&bef);
			// create ib + vb

			for (int ivb = 0; ivb < m_xObj.m_iNumFaces * 3; ++ivb)
			{
				PNCT_VERTEX v = mesh.m_tmpVertexList[ivb];
				int iPos = isEqualVertexList(mesh, v);
				if (iPos < 0)
				{
					mesh.m_VertexList.push_back(v);
					iPos = mesh.m_VertexList.size() - 1;
				}
				mesh.m_IndexList.push_back(iPos);
			}

			QueryPerformanceCounter(&aft);
			time = (float)(aft.LowPart - bef.LowPart) / (float)fre.LowPart;

			mesh.m_DxObject.m_iNumVertex = mesh.m_VertexList.size();
			mesh.m_DxObject.m_iNumIndex = mesh.m_IndexList.size();
			int iRef = m_obj.m_ObjectList[i].mtrlRef;
			std::tstring texName = m_obj.m_MateriaList[iRef].Texture[0].Filename;
			std::tstring texPath = m_obj.m_MateriaList[iRef].Texture[0].Filepath;

			mesh.m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
			mesh.Create(m_pd3dDevice, L"shape.hlsl", texPath + texName);
			m_xObj.m_Objectlist.push_back(mesh);
		}
	}
	QueryPerformanceCounter(&aft);
	time = (float)(aft.LowPart - bef0.LowPart) / (float)fre.LowPart;
	return true;
}
bool Sample::Frame()
{
	return true;
}
bool Sample::Render()
{
	for (auto& x : m_xObj.m_Objectlist)
	{
		x.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
		x.Render(m_pImmediateContext);
	}
	return true;
}
bool Sample::Release()
{
	return true;
}

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
