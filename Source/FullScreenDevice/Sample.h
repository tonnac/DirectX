#pragma once
#include "Core.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool					Init	() override;
	bool					Render	() override;
	bool					Frame	() override;
	bool					Release	() override;
private:
};