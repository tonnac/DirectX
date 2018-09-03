#pragma once
#include "Core.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool					PreInit	() override;
	bool					Init	() override;
	bool					Render	() override;
	bool					Release	() override;
private:
};