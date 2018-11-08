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
	std::array<D3D11_VIEWPORT, 4> m_vp;
	ObjectPlane	m_Obj;
};
