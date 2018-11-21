#pragma once

#include "Camera.h"

class Arcball
{
public:
	Arcball();
public:
	void OnBegin(int x, int y);
	void OnMove(int x, int y);
	void OnEnd();
	
	D3DXVECTOR3 ScreenToVector(float fX, float fY);

	D3DXQUATERNION QuatFromBallPoints(const D3DXVECTOR3 &vFrom, const D3DXVECTOR3 &vTo);

	D3DXMATRIX getRotationMat();
public:
	bool m_bDrag = false;

	D3DXVECTOR3 m_vDownPt;
	D3DXVECTOR3 m_vCurPt;

	D3DXQUATERNION m_pNow;
	D3DXQUATERNION m_pDown;
};
