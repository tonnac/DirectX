#pragma once
#include "ObjectPlane.h"

class Sphere
{
public:
	DX::DxObj m_DxObject;
	CB_DATA m_cbData;
	std::vector<PC_VERTEX> m_VertexList;
	std::vector<DWORD> m_indicies;
	UINT m_iIndex;
	UINT m_iVertex;
public:
	bool Init();
	bool Set(ID3D11Device* pDevice, float radius, int slicecount, int stackcount);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	Sphere();
	virtual ~Sphere();
};