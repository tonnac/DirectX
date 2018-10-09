#include "Sample.h"
#include "DirectInput.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	CreateRasterizerState();
	m_Object.Create(getDevice(), L"EPE", L"VertexShader.txt", L"../../data/effect/Particle6.dds");
	return true;
}
bool Sample::Frame()
{
	m_Object.Frame();
	return true;
}
void Sample::CreateRasterizerState()
{
	ID3D11Device* pDevice = getDevice();

	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));

	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = TRUE;
	pDevice->CreateRasterizerState(&desc, &m_pWFRasterizer);
	desc.FillMode = D3D11_FILL_SOLID;
	pDevice->CreateRasterizerState(&desc, &m_pRasterizer);
}
bool Sample::Render()
{
	ID3D11DeviceContext* pContext = getContext();
	pContext->RSSetState(m_pWFRasterizer);
	m_Object.Render(getContext());
	return true;
}
bool Sample::Release()
{
	m_Object.Release();
	return true;
}


int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
