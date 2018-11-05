#pragma once
#include "Core.h"
#include "ObjectPlane.h"

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
	ObjectPlane	m_Obj;
};
