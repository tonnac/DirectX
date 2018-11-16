#pragma once
#include "Define.h"
#include "dinput.h"

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

#define KEYDOWN(x) ((m_KeyState[(x)]) & 0x80 ? true : false)
#define KEYUP(x) ((m_KeyState[(x)]) & 0x80 ? false : true)

struct GameInput
{
	BOOL bFront;
	BOOL bBack;
	BOOL bLeft;
	BOOL bRight;
	BOOL bAttack;
	BOOL bJump;
};

class DirectInput : public Singleton<DirectInput>
{
	friend class Singleton<DirectInput>;
private:
	DirectInput();
public:
	bool					Init		();
	bool					Frame		();
	bool					Render		();
	bool					Release		();
	bool					PostProcess	();
public:
	KEYSTATE				getKeyState(const DWORD dwKey);
public:
	LPDIRECTINPUT8			m_pDi;
	LPDIRECTINPUTDEVICE8	m_pKey;
	LPDIRECTINPUTDEVICE8	m_pMouse;

	BYTE					m_BeforeKeyState[KEYSTATECOUNT];
	BYTE					m_KeyState[KEYSTATECOUNT];
	DIMOUSESTATE			m_MouseState;
	DIMOUSESTATE			m_BeforeMouseState;
};

#define S_Input DirectInput::getInstance()