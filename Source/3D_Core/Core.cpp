#include "Core.h"
#include <DirectXColors.h>
#include "DxState.h"

bool Core::GameInit()
{
#pragma region
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateSwapChain(m_hWnd, m_iWindowWidth, m_iWindowHeight))) return false;
	if (FAILED(SetRTVDSV())) return false;
	SetViewPort();
	DxState::InitState(m_pd3dDevice);

	m_DefaultCamera.SetViewMatrix({ 5,5,-10.0f });
	m_DefaultCamera.SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);

	m_pMainCamera = &m_DefaultCamera;

	m_Dir.Create(m_pd3dDevice, L"shape.hlsl");
#pragma endregion Device_Init

#ifdef DEVICE_INFO
	Enumeration::CreateDeviceInfo(getGIFactory());
#endif

#pragma region 
	IDXGISurface1 * pSurface = nullptr;
	IDXGISwapChain* pSwapChain = getSwapChain();
	pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pSurface);
	S_DirectWrite.Init();
	S_DirectWrite.Set(g_rtClient.right, g_rtClient.bottom, pSurface);
	pSurface->Release();
#pragma endregion Write_Init

	IDXGIFactory * pFactory = getGIFactory();
	pFactory->MakeWindowAssociation(g_hWnd, DXGI_MWA_NO_ALT_ENTER | DXGI_MWA_NO_WINDOW_CHANGES); // 윈도우 메시지와 ALT+ENTER로 인한 전체화면을 막음
	m_Timer.Init();
	S_Input.Init();
	Init();
	return true;
}
bool Core::GameRun()
{
	GameFrame();
	GameRender();
	return true;
}
bool Core::GameRelease()
{
	S_Texture.Release();
	S_Shader.Release();
	m_Timer.Release();
	S_Input.Release();
	Release();
	if (CleanupDevice() == false) return false;
	return true;
}
bool Core::GameFrame()
{
	S_Input.Frame();
	if (S_Input.getKeyState(DIK_A) == KEYSTATE::KEY_HOLD)
	{
		m_YawPitchRoll.y -= g_fSecPerFrame * 0.5f;
	}
	if (S_Input.getKeyState(DIK_D) == KEYSTATE::KEY_HOLD)
	{
		m_YawPitchRoll.y += g_fSecPerFrame * 0.5f;
	}
	if (S_Input.getKeyState(DIK_W) == KEYSTATE::KEY_HOLD)
	{
		m_YawPitchRoll.x -= g_fSecPerFrame * 0.5f;
	}
	if (S_Input.getKeyState(DIK_S) == KEYSTATE::KEY_HOLD)
	{
		m_YawPitchRoll.x += g_fSecPerFrame * 0.5f;
	}
	m_pMainCamera->Update(m_YawPitchRoll);
	m_pMainCamera->Frame();
	m_Timer.Frame();
	Frame();
	S_Input.PostProcess();
	return true;
}
bool Core::GameRender()
{
	if (PreRender() == false) return false;
	m_pMainCamera->SetProjMatrix(m_pMainCamera->m_fFov, m_pMainCamera->m_fAspect);
//	m_pMainCamera->SetViewMatrix(m_pMainCamera->m_vPos);
	m_Dir.SetMatrix(nullptr, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_Dir.Render(m_pImmediateContext);
	Render();
	m_Timer.Render();
	if (PostRender() == false) return false;
	return true;
}
bool Core::PreInit()
{
	return true;
}
void Core::DeleteDeviceResources()
{
	S_DirectWrite.DiscardDeviceResources();
	DeleteResources();
}
HRESULT Core::CreateDeviceResources(const UINT& Width, const UINT& Height)
{
	HRESULT hr;
	IDXGISurface1 * pBackBuffer = nullptr;
	IDXGISwapChain * pSwapChain = getSwapChain();
	hr = pSwapChain->GetBuffer(0, __uuidof(IDXGISurface1), (LPVOID*)&pBackBuffer);
	if (FAILED(hr)) return false;
	S_DirectWrite.CreateDeviceResources(pBackBuffer);
	pBackBuffer->Release();

//	m_pMainCamera->UpdateProjMatrix(Width, Height);
	CreateResources(Width, Height);
	return hr;
}
void Core::DeleteResources()
{
	return;
}
HRESULT	Core::CreateResources(const UINT& Width, const UINT& Height)
{
	return S_OK;
}
bool Core::Init() { return true; }
bool Core::Frame() { return true; }
bool Core::Render() { return true; }
bool Core::Release() { return true; }
bool Core::PreRender()
{
	m_pImmediateContext->ClearRenderTargetView(m_pRenderTargetView, DirectX::Colors::LightSteelBlue);

	m_pImmediateContext->RSSetState(DxState::m_RSS[(int)E_RSS::Default].Get());
	m_pImmediateContext->OMSetBlendState(DxState::m_BSS[(int)E_BSS::Default].Get(), 0, -1);
	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)E_DSS::Default].Get(), 0);
	m_pImmediateContext->PSSetSamplers(0, 1, DxState::m_SS[(int)E_SS::Default].GetAddressOf());

	m_pImmediateContext->ClearDepthStencilView(m_pDepthStencilView, D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	return true;
}
bool Core::PostRender()
{
	m_pSwapChain->Present(0, 0);
	return true;
}
