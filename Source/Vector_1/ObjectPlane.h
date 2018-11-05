#pragma once
#include "Core.h"
#include "Vector.h"
#include "DxObject.h"

struct PC_VERTEX
{
	Vector3 p;
	Vector4 c;
};

struct CB_DATA
{
	float fTime;
	float y, z, w;
};

class ObjectPlane
{
public:
	DX::DxObject m_DxObject;
	CB_DATA m_cbData;
	std::array<PC_VERTEX, 6> m_VertexList;
	std::array<DWORD, 12> m_indicies;
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