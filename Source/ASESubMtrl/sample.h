#pragma once
#include "Core.h"
#include "Mesh.h"
#include "AseMesh.h"


class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	int			isEqualVertexList(Mesh& mesh, PNCT_VERTEX& v);
	bool	   	Init() override;
	bool	   	Frame() override;
	bool	   	Render() override;
	bool	   	Release() override;
private:
	AseMesh m_obj;
	std::array<Mesh, 2> m_xObj;
};
