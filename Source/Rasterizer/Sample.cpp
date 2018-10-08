#include "Sample.h"
#include "DirectInput.h"


Sample::Sample()
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	SetRasterizerState();
	m_Object.Create(getDevice(), L"EPE", L"VertexShader.txt", L"../../data/effect/Particle5.dds");
	return true;
}
bool Sample::Frame()
{
	ID3D11DeviceContext* pContext = getContext();
	if (S_Input.getKeyState(DIK_E) == KEYSTATE::KEY_HOLD)
	{
		pContext->RSSetState(m_pRSWireFrame);
	}
	else
	{
		pContext->RSSetState(m_pRSSolid);
	}
	m_Object.Frame();
	return true;
}
bool Sample::Render()
{
	m_Object.Render(getContext());
	return true;
}
bool Sample::Release()
{
	m_Object.Release();
	return true;
}
HRESULT Sample::SetRasterizerState(const D3D11_FILL_MODE& fill)
{
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();
	D3D11_RASTERIZER_DESC desc;
	ZeroMemory(&desc, sizeof(D3D11_RASTERIZER_DESC));
	desc.FillMode = D3D11_FILL_WIREFRAME;
	desc.CullMode = D3D11_CULL_NONE;
	desc.DepthClipEnable = TRUE;

	V_RETURN(pDevice->CreateRasterizerState(&desc, &m_pRSWireFrame));
	desc.FillMode = D3D11_FILL_SOLID;
	V_RETURN(pDevice->CreateRasterizerState(&desc, &m_pRSSolid));
	return hr;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleLibrary");
	wd.Run();
	return 0;
}
