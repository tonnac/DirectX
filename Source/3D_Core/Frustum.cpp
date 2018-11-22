#include "Frustum.h"

bool Frustum::CreateFrustum()
{
	D3DXMATRIX matInvViewProj;
	D3DXMatrixMultiply(&matInvViewProj, &m_View, &m_Proj);

	D3DXMatrixInverse(&matInvViewProj, nullptr, &matInvViewProj);

	m_vFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, +0.0f);
	m_vFrustum[1] = D3DXVECTOR3(-1.0f, +1.0f, +0.0f);
	m_vFrustum[2] = D3DXVECTOR3(+1.0f, +1.0f, +0.0f);
	m_vFrustum[3] = D3DXVECTOR3(+1.0f, -1.0f, +0.0f);

	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	m_vFrustum[5] = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	m_vFrustum[6] = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	m_vFrustum[7] = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);

	for (int iVertex = 0; iVertex < 8; ++iVertex)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex], &m_vFrustum[iVertex], &matInvViewProj);
	}

	m_Plane[0].CreatePlane(m_vFrustum[0], m_vFrustum[1], m_vFrustum[2]);
	m_Plane[1].CreatePlane(m_vFrustum[6], m_vFrustum[5], m_vFrustum[4]);

	m_Plane[2].CreatePlane(m_vFrustum[2], m_vFrustum[6], m_vFrustum[7]);
	m_Plane[3].CreatePlane(m_vFrustum[5], m_vFrustum[1], m_vFrustum[0]);

	m_Plane[4].CreatePlane(m_vFrustum[1], m_vFrustum[5], m_vFrustum[6]);
	m_Plane[5].CreatePlane(m_vFrustum[0], m_vFrustum[3], m_vFrustum[7]);

	return true;
}

bool Frustum::ClassifyPoint(D3DXVECTOR3 v)
{
	for (int i = 0; i < 6; ++i)
	{
		D3DXVECTOR3 n = { m_Plane[i].a, m_Plane[i].b, m_Plane[i].c };
	}

	return true;
}

bool Frustum::ClassifySphere(D3DXVECTOR3 v)
{
	return true;
}

void Frustum::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != nullptr)
	{
		m_World = *pWorld;
	}
	if (pView != nullptr)
	{
		m_View = *pView;
	}
	if (pProj != nullptr)
	{
		m_Proj = *pProj;
	}
}