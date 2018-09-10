#include "DirectInput.h"

GameInput g_Input;

DirectInput::DirectInput()
{
	ZeroMemory(&m_pKey, sizeof(BYTE) * 256);
	ZeroMemory(&m_MouseState, sizeof(DIMOUSESTATE));
}
bool DirectInput::Init()
{
	HRESULT hr;
	hr = DirectInput8Create(g_hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, (LPVOID*)&m_pDi, NULL);
	if (FAILED(hr)) return false;

	// KEYBOARD
	hr = m_pDi->CreateDevice(GUID_SysKeyboard, &m_pKey, NULL);
	if (FAILED(hr)) return false;
	hr = m_pKey->SetDataFormat(&c_dfDIKeyboard);
	if (FAILED(hr)) return false;
	hr = m_pKey->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND | DISCL_NOWINKEY);
	if (FAILED(hr)) return false;
	while (m_pKey->Acquire() == DIERR_INPUTLOST);
	// MOUSE
	hr = m_pDi->CreateDevice(GUID_SysMouse, &m_pMouse, NULL);
	if (FAILED(hr)) return false;
	hr = m_pMouse->SetDataFormat(&c_dfDIMouse);
	if (FAILED(hr)) return false;
	hr = m_pMouse->SetCooperativeLevel(g_hWnd, DISCL_NONEXCLUSIVE | DISCL_FOREGROUND);
	if (FAILED(hr)) return false;
	while (m_pMouse->Acquire() == DIERR_INPUTLOST);

	return true;
}
bool DirectInput::Frame()
{
	HRESULT hr;
	hr = m_pKey->GetDeviceState(KEYSTATECOUNT, &m_KeyState);
	if (FAILED(hr))
	{
		while (m_pKey->Acquire() == DIERR_INPUTLOST);
	}
	hr = m_pMouse->GetDeviceState(sizeof(DIMOUSESTATE), &m_MouseState);
	if (FAILED(hr))
	{
		while (m_pMouse->Acquire() == DIERR_INPUTLOST);
	}
	g_Input.bFront = m_KeyState[DIK_W];
	g_Input.bBack = m_KeyState[DIK_S];
	g_Input.bLeft = m_KeyState[DIK_A];
	g_Input.bRight = m_KeyState[DIK_D];
	g_Input.bJump = m_KeyState[DIK_SPACE] | m_MouseState.rgbButtons[1];
	g_Input.bAttack = m_KeyState[DIK_RETURN] | m_MouseState.rgbButtons[0];
	return true;
}
bool DirectInput::Render()
{
	return true;
}
bool DirectInput::Release()
{
	m_pKey->Unacquire();
	m_pMouse->Unacquire();

	m_pKey->Release();
	m_pMouse->Release();
	m_pDi->Release();

	m_pKey = nullptr;
	m_pMouse = nullptr;
	m_pDi = nullptr;
	return true;
}
bool DirectInput::PostProcess()
{
	memcpy(m_BeforeKeyState, m_KeyState, sizeof(BYTE)* KEYSTATECOUNT);
	memcpy(&m_BeforeMouseState, &m_MouseState, sizeof(DIMOUSESTATE));
	return true;
}
KEYSTATE DirectInput::getKeyState(const DWORD dwKey)
{
	if (m_BeforeKeyState[dwKey] & 0x80)
	{
		if (KEYDOWN(dwKey))
		{
			return KEYSTATE::KEY_HOLD;
		}
		else
		{
			return KEYSTATE::KEY_UP;
		}
	}
	else
	{
		if (KEYDOWN(dwKey))
		{
			return KEYSTATE::KEY_PUSH;
		}
		else
		{
			return KEYSTATE::KEY_FREE;
		}
	}
}