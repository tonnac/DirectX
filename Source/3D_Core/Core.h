#pragma once
#include "Window.h"
#include "Enumeration.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"
#include "Texture.h"
#include "Shader.h"
#include "Shape.h"
#include "DxState.h"
#include "ModelView.h"

//#define DEVICE_INFO

extern GameInput g_Input;

class Core : public Window
{
public:
	Core() 
	{
		m_Camera.clear();
	};
public:
	bool			GameInit				() override;
	bool			GameRun					() override;
	bool			GameRelease				() override;
	bool			GameFrame				();
	bool			GameRender				();
	virtual bool	PreInit					();
	void			DeleteDeviceResources	() override;
	HRESULT			CreateDeviceResources	(const UINT& Width, const UINT& Height) override;
	virtual void	DeleteResources			();
	virtual HRESULT	CreateResources			(const UINT& Width, const UINT& Height);

	virtual LRESULT  WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)override;
public:
	virtual bool	Init					();
	virtual bool	Frame					();
	virtual bool	Render					();
	virtual bool	Release					();

protected:
	void			CreateCamera			();

private:
	bool			PreRender				();
	bool			PostRender				();

	D3DXVECTOR4		OnKeyboardInput();

protected:
	Timer			m_Timer;
	Camera*			m_pMainCamera;
	DirectionShape  m_Dir;

	std::unordered_map<std::string, std::unique_ptr<Camera>> m_Camera;
	E_DSS m_DepthStencilState = E_DSS::Default;
	E_RSS m_RasterizerState = E_RSS::Default;
	E_BSS m_BlendState = E_BSS::Default;
	E_SS  m_SampleState = E_SS::Default;
};