#pragma once
#include "Window.h"

class Sample : public Window
{
public:
	Sample();
	~Sample();
public:
	bool					Init() override;
	bool					Render() override;
	bool					Release() override;
private:

};