#pragma once
#include "Core.h"
#include "ObjectPlane.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	HRESULT		CreateDSV();
	bool	   	Init() override;
	bool	   	Frame() override;
	bool	   	Render() override;
	bool	   	Release() override;
private:
	ID3D11DepthStencilView* m_pDSV;
	ID3D11DepthStencilState* m_pDSVStateEnable;
	ID3D11DepthStencilState* m_pDSVStateDisable;

	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	ObjectPlane	m_Obj;
};
