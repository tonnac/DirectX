#pragma once
#include "Core.h"
#include "Shape.h"
#include "Camera.h"
#include "ModelView.h"

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

	void		ViewportRender(Shape * pShape);
private:
	D3D11_VIEWPORT m_vp[3];

	BoxShape	m_boxObj;
	ModelView*  m_pModelCamera;
};
