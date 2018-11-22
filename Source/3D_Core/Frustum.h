#pragma once
#include "Define.h"

struct Plane
{
	float a = 0;
	float b = 0;
	float c = 0;
	float d = 0;

	void CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
	{
		D3DXVECTOR3 vEdge0 = v1 - v0;
		D3DXVECTOR3 vEdge1 = v2 - v0;
		D3DXVECTOR3 vNormal;
		D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);

		D3DXVec3Normalize(&vNormal, &vNormal);
		a = vNormal.x;
		b = vNormal.y;
		c = vNormal.z;
		c = D3DXVec3Dot(&v0, &vNormal);
	}

	//void CreatePlane(D3DXVECTOR3 v0, D3DXVECTOR3 v1, D3DXVECTOR3 v2)
	//{
	//	D3DXVECTOR3 vEdge0 = v1 - v0;
	//	D3DXVECTOR3 vEdge1 = v2 - v0;
	//	D3DXVECTOR3 vNormal;
	//	D3DXVec3Cross(&vNormal, &vEdge0, &vEdge1);

	//	D3DXVec3Normalize(&vNormal, &vNormal);
	//	a = vNormal.x;
	//	b = vNormal.y;
	//	c = vNormal.z;
	//	c = D3DXVec3Dot(&v0, &vNormal);
	//}
};

class Frustum
{
public:
	bool CreateFrustum();
	void SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj);
	bool ClassifyPoint(D3DXVECTOR3 v);
	bool ClassifySphere(D3DXVECTOR3 v);

public:
	Plane m_Plane[6];

	D3DXVECTOR3 m_vFrustum[8];

	D3DXMATRIX m_World;
	D3DXMATRIX m_View;
	D3DXMATRIX m_Proj;
};