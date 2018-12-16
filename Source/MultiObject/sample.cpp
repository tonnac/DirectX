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
	steady_clock::time_point bef0 = steady_clock::now();
	steady_clock::time_point aft;
	duration<double> t;
	if (m_obj.Load(L"turret.ase"))
	{
		m_mesh = m_obj.Convert(m_pd3dDevice);
	}
	aft = steady_clock::now();
	t = aft - bef0;
	return true;
}
bool Sample::Frame()
{
	m_mesh->Frame();
	return true;
}
bool Sample::Render()
{
	m_mesh->SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_mesh->Render(m_pImmediateContext);
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
