#pragma once
#include "Core.h"
#include "ZXCLoader.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool	   	Init() override;
	bool	   	Frame() override;
	bool	   	Render() override;
	bool	   	Release() override;
private:
	std::unique_ptr<Mesh> m_mesh = nullptr;
};
