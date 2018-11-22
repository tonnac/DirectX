#pragma once
#include "Frustum.h"

class Camera : public Frustum
{

public:
	Camera();
	virtual ~Camera();

public:
	D3DXMATRIX SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget = D3DXVECTOR3(0,0,0), D3DXVECTOR3 vUp = D3DXVECTOR3(0,1,0));
	D3DXMATRIX SetProjMatrix(float fFov, float fAspect, float fNear = 1.0f, float fFar = 1000.0f);

	void UpdateProjMatrix(UINT Width, UINT Height);

	bool Frame();
	virtual bool Update(D3DXVECTOR4 vValue);
	void UpdateVector();
	void MoveLook(float fValue);
	void MoveSide(float fValue);

public:
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	float m_fFov;
	float m_fAspect;
	float m_fNear;
	float m_fFar;
	float m_fSpeed = 1.0f;

	D3DXVECTOR3 m_vPos;
	D3DXVECTOR3 m_vTarget;
	D3DXVECTOR3 m_vUp;

	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;
	D3DXVECTOR3 m_vUpvector;

	float m_fCameraYawAngle;
	float m_fCameraPitchAngle;

	D3DXMATRIX m_matModelRot;
	D3DXMATRIX m_matModelLast;
};