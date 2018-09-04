#pragma once
#include "Core.h"
#include "Timer.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool					PreInit	() override;
	bool					Init	() override;
	bool					Render	() override;
	bool					Frame	() override;
	bool					Release	() override;
private:
	Timer					m_Timer;
};