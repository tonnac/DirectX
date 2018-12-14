#pragma once

#include "Shape.h"

class Mesh : public Shape
{
public:
	int m_iNumFaces;
	std::vector<Mesh> m_Objectlist;
	std::vector<PNCT_VERTEX> m_tmpVertexList;

	virtual HRESULT CreateVertexData()override;
	virtual HRESULT CreateIndexData()override;
};
