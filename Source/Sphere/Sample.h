#pragma once
#include "Core.h"
#include "Sphere.h"

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
	D3DXMATRIX m_matWorld[2];
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;
	
	D3D11_VIEWPORT m_vs[2];

	ID3D11RasterizerState* m_pRasterizer;
	Sphere m_sphere;
//	ObjectPlane	m_Obj;
};
