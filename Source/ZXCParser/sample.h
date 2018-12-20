#pragma once
#include "Core.h"
#include "Mesh.h"

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
	ZXCMesh m_obj;
	std::array<Mesh, 2> m_xObj;

	std::unique_ptr<Mesh> m_mesh = nullptr;
};
