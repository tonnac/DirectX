#include "Arcball.h"

Arcball::Arcball()
{
	D3DXQuaternionIdentity(&m_pNow);
	D3DXQuaternionIdentity(&m_pDown);
	m_vDownPt = { 0, 0, 0 };
	m_vCurPt = { 0,0,0 };
}

D3DXMATRIX Arcball::getRotationMat()
{
	D3DXMATRIX matRot;
	D3DXMatrixRotationQuaternion(&matRot, &m_pNow);
	return matRot;
}

void Arcball::OnBegin(int x, int y)
{
	m_bDrag = true;
	m_pDown = m_pNow;
	m_vDownPt = ScreenToVector((float)x, (float)y);
}

void Arcball::OnMove(int x, int y)
{
	if (m_bDrag)
	{
		m_vCurPt = ScreenToVector((float)x, (float)y);
		m_pNow = m_pDown * QuatFromBallPoints(m_vDownPt, m_vCurPt);
	}
}

void Arcball::OnEnd()
{
	m_bDrag = false;
}

D3DXVECTOR3 Arcball::ScreenToVector(float fX, float fY)
{
	float x = -((fX - g_rtClient.right * 0.5f) / (g_rtClient.right * 0.5f));
	float y = (fY - (g_rtClient.bottom * 0.5f)) / (g_rtClient.bottom * 0.5f);
	float z = 0.0f;

	float fFlag = (x*x) + (y*y);

	if (fFlag > 1.0f)
	{
		x /= sqrtf(fFlag);
		y /= sqrtf(fFlag);
	}
	else
	{
		z = sqrtf(1.0f - fFlag);
	}

	return { x, y, z };
}

D3DXQUATERNION Arcball::QuatFromBallPoints(const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTo)
{
	float fDot = D3DXVec3Dot(&vFrom, &vTo);

	D3DXVECTOR3 part;
	D3DXVec3Cross(&part, &vFrom, &vTo);
	return { part.x, part.y ,part.z, fDot };
}