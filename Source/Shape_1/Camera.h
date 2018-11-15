#pragma once
#include "Define.h"

class Camera
{

public:
	Camera();
	virtual ~Camera();

public:
	D3DXMATRIX SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget = D3DXVECTOR3(0,0,0), D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0));
	D3DXMATRIX SetProjMatrix(float fFov, float fAspect, float fNear = 1.0f, float fFar = 1000.0f);

	void UpdateProjMatrix(UINT Width, UINT Height);

public:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	float m_fFov;
	float m_fAspect;
	float m_fNear;
	float m_fFar;

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vUp;
};