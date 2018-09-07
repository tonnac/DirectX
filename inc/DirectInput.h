#pragma once
#include "Define.h"
#include "dinput.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

struct GameInput
{
	BOOL bFront;
	BOOL bBack;
	BOOL bLeft;
	BOOL bRight;
	BOOL bAttack;
	BOOL bJump;
};

class DirectInput
{
public:
	DirectInput();
public:
	bool					Init	();
	bool					Frame	();
	bool					Render	();
	bool					Release	();
private:
	LPDIRECTINPUT8			m_pDi;
	LPDIRECTINPUTDEVICE8	m_pKey;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	BYTE					m_KeyState[KEYSTATECOUNT];
	DIMOUSESTATE			m_MouseState;
};

