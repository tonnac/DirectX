#pragma once
#include "Core.h"
#include "Vector.h"
#include "DxObj.h"

struct PC_VERTEX
{
	D3DXVECTOR3 p;
	D3DXVECTOR4 c;
};

struct CB_DATA
{
	D3DXMATRIX matWorld;
	D3DXMATRIX matView;
	D3DXMATRIX matProj;
	float fTime;
	float fActiveVP;
	float z, w;
	D3DXVECTOR4 vColor;
};

class ObjectPlane
{
public:
	DX::DxObj m_DxObject;
	CB_DATA m_cbData;
	std::array<PC_VERTEX, 9> m_VertexList;
	std::array<DWORD, 24> m_indicies;
	UINT m_iIndex;
	UINT m_iVertex;
public:
	bool Init();
	bool Set(ID3D11Device* pDevice);
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();
public:
	ObjectPlane();
	virtual ~ObjectPlane();
};