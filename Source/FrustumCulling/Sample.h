#pragma once
#include "Core.h"
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

	std::array<D3DXMATRIX, 100> m_worldarray;

	DxRT m_dxrt1;
	PlaneShape m_Plane;
	BoxShape m_boxObj;
	Frustum m_Frustum;

	int inFrustumCnt = 0;
};
