#include "Sample.h"
#include "DirectInput.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	m_Obj.Init();
	m_Obj.Set(getDevice());
	for (int i = 0; i < 4; ++i)
	{
		m_vp[i].Width = (float)g_rtClient.right / 2;
		m_vp[i].Height = (float)g_rtClient.bottom / 2;
		m_vp[i].MaxDepth = 1.0f;
		m_vp[i].MinDepth = 0;
	}
	m_vp[0].TopLeftX = 0;
	m_vp[0].TopLeftY = 0;

	m_vp[1].TopLeftX = (float)g_rtClient.right / 2;
	m_vp[1].TopLeftY = 0;

	m_vp[2].TopLeftX = 0;
	m_vp[2].TopLeftY = (float)g_rtClient.bottom / 2;

	m_vp[3].TopLeftX = (float)g_rtClient.right / 2;
	m_vp[3].TopLeftY = (float)g_rtClient.bottom / 2;
	return true;
}
bool Sample::Frame()
{
	m_Obj.Frame();
	return true;
}
bool Sample::Render()
{
	m_Obj.m_cbData.fActiveVP = 0;
	getContext()->RSSetViewports(1, &m_vp[0]);
	m_Obj.Render(getContext());

	m_Obj.m_cbData.fActiveVP = 0;
	getContext()->RSSetViewports(1, &m_vp[1]);
	m_Obj.Render(getContext());

	getContext()->RSSetViewports(2, &m_vp[2]);
	m_Obj.m_cbData.fActiveVP = 0;
	getContext()->GSSetConstantBuffers(0, 1, m_Obj.m_DxObject.m_pConstantBuffer.GetAddressOf());
	m_Obj.Render(getContext());

	m_Obj.m_cbData.fActiveVP = 1;
	m_Obj.Render(getContext());
	return true;
}
bool Sample::Release()
{
	m_Obj.Release();
	return true;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
