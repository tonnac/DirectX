#pragma once
#include "Core.h"
#include "DirectInput.h"

extern GameInput g_Input;

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool		Init	() override;
	bool		Render	() override;
	bool		Release	() override;
private:
	DirectInput m_Dinput;
};