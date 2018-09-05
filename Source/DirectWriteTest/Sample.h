#pragma once
#include "Core.h"
#include "DirectWrite.h"
class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool					Init	() override;
	bool					Render	() override;
	bool					Release	() override;
private:
};