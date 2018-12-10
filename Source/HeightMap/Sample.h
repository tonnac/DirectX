#pragma once
#include "Core.h"
#include "Map.h"
#include "Frustum.h"


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
	D3D11_VIEWPORT m_vp;

	std::array<D3DXMATRIX, 100> m_matWorld;

	PlaneShape  m_Plane;
	BoxShape	m_boxObj;
	Frustum		m_Frustum;
	
	DxRT		m_dxrt1;

	Map			m_Map;

	int inFrustumCnt = 0;
};