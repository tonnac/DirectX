#pragma once
#include "Window.h"

class Core : public Window
{
public:
	bool			GameInit		() override;
	bool			GameRun			() override;
	bool			GameRelease		() override;
	bool			GameFrame		();
	bool			GameRender		();
	virtual bool	PreInit			();
public:
	virtual bool	Init			();
	virtual bool	Frame			();
	virtual bool	Render			();
	virtual bool	Release			();
private:
	bool			GamePreRender	();
	bool			GamePostRender	();
};