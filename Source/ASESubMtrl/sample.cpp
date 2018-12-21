#include "Sample.h"
#include "DirectInput.h"
#include "../inc/DxState.h"
#include <chrono>
using namespace DirectX;

using namespace std::chrono;

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
	steady_clock c1;
	
	steady_clock::time_point bef0 = c1.now();
	steady_clock::time_point bef;
	steady_clock::time_point aft;
	duration<double> t;

	bef = c1.now();
	if (m_obj.Load(L"st02sc00.ase"))
	{
		aft = c1.now();

		t = aft - bef;

		bef = c1.now();

		for (size_t i = 0; i < m_obj.m_ObjectList.size(); ++i)
		{
			m_xObj[i].m_Objectlist.resize(m_obj.m_MateriaList[i].SubMaterial.size());
			m_xObj[i].m_iNumFaces = (int)m_obj.m_ObjectList[i].posFaceList.size();
			for (int iFace = 0; iFace < m_xObj[i].m_iNumFaces; ++iFace)
			{
				int MtrlIndex = m_obj.m_ObjectList[i].posFaceList[iFace].Mtrl;
				auto& mesh = m_xObj[i].m_Objectlist[MtrlIndex];
				for (int iVer = 0; iVer < 3; ++iVer)
				{
					int vID = iFace * 3 + iVer;
					PNCT_VERTEX vertex;
					//pos
					if (m_obj.m_ObjectList[i].vertexList.size() > 0)
					{
						int index = m_obj.m_ObjectList[i].posFaceList[iFace].v[iVer];
						vertex.p = (float*)&m_obj.m_ObjectList[i].vertexList[index];
					}

					//normal
					if (m_obj.m_ObjectList[i].norList.size() > 0)
					{
						vertex.n = (float*)&m_obj.m_ObjectList[i].norList[vID];
					}

					//color
					if (m_obj.m_ObjectList[i].colorFaceList.size() > 0)
					{
						int index = m_obj.m_ObjectList[i].colorFaceList[iFace].v[iVer];
						vertex.c = (float*)&m_obj.m_ObjectList[i].colorList[index];
						vertex.c.w = 1.0f;
					}

					//tex
					if (m_obj.m_ObjectList[i].texFaceList.size() > 0)
					{
						int index = m_obj.m_ObjectList[i].texFaceList[iFace].v[iVer];
						vertex.t = (float*)&m_obj.m_ObjectList[i].texList[index];
					}
					mesh.m_VertexList.push_back(std::move(vertex));
				}
			}
			aft = c1.now();
			t = bef - aft;

			bef = c1.now();
			for (size_t k = 0; k < m_xObj[i].m_Objectlist.size(); ++k)
			{
				auto& obj = m_xObj[i].m_Objectlist[k];
				std::tstring texName = m_obj.m_MateriaList[i].SubMaterial[k].Texture[0].Filename;
				std::tstring texPath = L"..\\..\\data\\tex\\";
				obj.m_DxObject.m_iNumVertex = (UINT)obj.m_VertexList.size();
				obj.m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
				obj.Create(m_pd3dDevice, L"shape.hlsl", texPath + texName);
			}
		}
	}
	aft = c1.now();
	t = aft - bef0;
	return true;
}
bool Sample::Frame()
{
	return true;
}
bool Sample::Render()
{
	for (int i = 0; i < (int)m_xObj.size(); ++i)
	{
		for (auto& x : m_xObj[i].m_Objectlist)
		{
			x.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
			x.Render(m_pImmediateContext);
		}
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
