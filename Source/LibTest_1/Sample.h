#pragma once
#include "Core.h"
#include "Shape.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool	   	Init() override;
	bool	   	Frame() override;
	bool	   	Render() override;
	bool	   	Release() override;

	void		DeleteResources()override;
	HRESULT		CreateResources(const UINT& Width, const UINT& Height)override;

	LRESULT		WndProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)override;
private:
	BoxShape	m_boxObj;
};
