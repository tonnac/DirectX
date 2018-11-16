#pragma once
#include "Window.h"
#include "Enumeration.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"
#include "Texture.h"
#include "Shader.h"
#include "Camera.h"
#include "Shape.h"

//#define DEVICE_INFO

class Core : public Window
{
public:
	Core() {};
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
public:
	virtual bool	Init					();
	virtual bool	Frame					();
	virtual bool	Render					();
	virtual bool	Release					();
private:
	bool			PreRender				();
	bool			PostRender				();
protected:
	Timer			m_Timer;
	Camera*			m_pMainCamera;
	Camera			m_DefaultCamera;
	DirectionShape  m_Dir;
	D3DXVECTOR4		m_YawPitchRoll;
};