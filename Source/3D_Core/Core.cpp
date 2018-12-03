#include "Core.h"
#include <DirectXColors.h>

using namespace DirectX;

bool Core::GameInit()
{
#pragma region
	if (FAILED(CreateDevice())) return false;
	if (FAILED(CreateGIFactory())) return false;
	if (FAILED(CreateSwapChain(m_hWnd, m_iWindowWidth, m_iWindowHeight))) return false;
	if (FAILED(SetRTVDSV())) return false;
	DxState::InitState(m_pd3dDevice);
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

	CreateCamera();
	m_Dir.Create(m_pd3dDevice, L"shape.hlsl");
	m_SkyBox.Create(m_pd3dDevice, L"sky.hlsl", L"../../data/sky/grassenvmap1024.dds");
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
	m_Timer.Frame();
	S_Input.Frame();
	D3DXVECTOR4 input = OnKeyboardInput();
	for (auto &x : m_Camera)
	{
		auto cam = x.second.get();
		cam->Update(input);
	}
	m_pMainCamera->Frame();

	Frame();
	S_Input.PostProcess();
	return true;
}
bool Core::GameRender()
{
	if (PreRender() == false) return false;
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

	for (auto &x : m_Camera)
	{
		auto cam = x.second.get();
		cam->UpdateProjMatrix(Width, Height);
	}
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

LRESULT Core::WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	if(m_Camera["modelview"] != nullptr)
		m_Camera["modelview"]->MsgProc(hwnd, msg, wparam, lparam);
	return Window::WndProc(hwnd, msg, wparam, lparam);
}

bool Core::Init() { return true; }
bool Core::Frame() { return true; }
bool Core::Render() { return true; }
bool Core::Release() { return true; }

void Core::CreateCamera()
{
	auto defaultcamera = std::make_unique<Camera>();
	defaultcamera->SetViewMatrix({ 0,150,-10.0f });
	defaultcamera->SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);

	auto modelViewCam = std::make_unique<ModelView>();
	modelViewCam->SetViewMatrix({ 0,0,-10.0f });
	modelViewCam->SetProjMatrix((float)D3DX_PI * 0.25f, (float)g_rtClient.right / g_rtClient.bottom);

	m_Camera["default"] = std::move(defaultcamera);
	m_Camera["modelview"] = std::move(modelViewCam);

	m_pMainCamera = m_Camera["default"].get();
}

bool Core::PreRender()
{
	m_dxRt.Begin(m_pImmediateContext, D3DXVECTOR4(Colors::LightSteelBlue));

	m_pImmediateContext->PSSetSamplers(0, 1, DxState::m_SS[(int)E_SS::Point].GetAddressOf());

	D3DXMATRIX scale;
	D3DXMatrixScaling(&scale, 20, 20, 20);
	m_SkyBox.SetMatrix(&scale, &m_pMainCamera->m_matView, &m_pMainCamera->m_matProj);
	m_SkyBox.Render(m_pImmediateContext);

	m_pImmediateContext->RSSetState(DxState::m_RSS[(int)m_RasterizerState].Get());
	m_pImmediateContext->OMSetDepthStencilState(DxState::m_DSS[(int)m_DepthStencilState].Get(), 0);
	m_pImmediateContext->PSSetSamplers(0, 1, DxState::m_SS[(int)m_SampleState].GetAddressOf());
	m_pImmediateContext->OMSetBlendState(DxState::m_BSS[(int)m_BlendState].Get(), 0, -1);

	return true;
}
bool Core::PostRender()
{
	m_dxRt.End(m_pImmediateContext);
	m_pSwapChain->Present(0, 0);
	return true;
}

D3DXVECTOR4 Core::OnKeyboardInput()
{
	D3DXVECTOR4 YawPitchRoll(0, 0, 0, 0);
	if (S_Input.getKeyState(DIK_1) == KEYSTATE::KEY_PUSH)
	{
		IncreaseEnum(m_RasterizerState);
	}

	if (S_Input.getKeyState(DIK_2) == KEYSTATE::KEY_PUSH)
	{
		IncreaseEnum(m_DepthStencilState);
	}

	if (S_Input.getKeyState(DIK_3) == KEYSTATE::KEY_PUSH)
	{
		IncreaseEnum(m_BlendState);
	}

	if (S_Input.getKeyState(DIK_4) == KEYSTATE::KEY_PUSH)
	{
		IncreaseEnum(m_SampleState);
	}

	if (S_Input.getKeyState(DIK_A) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveSide(-g_fSecPerFrame * 15.0f);
	}

	if (S_Input.getKeyState(DIK_D) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveSide(g_fSecPerFrame * 15.0f);
	}

	if (S_Input.getKeyState(DIK_W) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveLook(g_fSecPerFrame * 15.0f);
	}

	if (S_Input.getKeyState(DIK_S) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->MoveLook(-g_fSecPerFrame * 15.0f);
	}

	if (S_Input.m_MouseState.rgbButtons[1])
	{
		YawPitchRoll.x += 0.3f * (float)D3DXToRadian(S_Input.m_MouseState.lY);
		YawPitchRoll.y += 0.3f * (float)D3DXToRadian(S_Input.m_MouseState.lX);
	}

	if (S_Input.getKeyState(DIK_SPACE) == KEYSTATE::KEY_HOLD)
	{
		m_pMainCamera->m_fSpeed += g_fSecPerFrame * 5.0f;
	}

	float fValue = (float)S_Input.m_MouseState.lZ * 20.0f;
	YawPitchRoll.w = fValue * g_fSecPerFrame;
	return YawPitchRoll;
}