#pragma once
#include "Window.h"
#include "Enumeration.h"
#include "Timer.h"
#include "DirectWrite.h"
#include "DirectInput.h"
#include "Texture.h"
#include "Shader.h"
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
	virtual void	DeleteDeviceResources	() override;
	virtual HRESULT	CreateDeviceResources	(const UINT& Width, const UINT& Height) override;
public:
	virtual bool	Init					();
	virtual bool	Frame					();
	virtual bool	Render					();
	virtual bool	Release					();
private:
	bool			PreRender				();
	bool			PostRender				();
private:
	Timer			m_Timer;
};