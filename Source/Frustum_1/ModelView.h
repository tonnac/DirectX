#pragma once

#include "Arcball.h"

class ModelView : public Camera
{
public:
	ModelView();
public:
	Arcball	m_worldArcball;
public:
	bool Update(D3DXVECTOR4 vValue)override;
	LRESULT MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
};
