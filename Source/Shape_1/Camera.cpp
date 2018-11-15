#include "Camera.h"

Camera::Camera()
{

}

Camera::~Camera()
{

}

D3DXMATRIX Camera::SetViewMatrix(D3DXVECTOR3 vPos, D3DXVECTOR3 vTarget, D3DXVECTOR3 vUp)
{
	m_vPos = vPos; m_vTarget = vTarget; m_vUp = vUp;
	D3DXMatrixLookAtLH(&m_matView,
		&vPos,
		&vTarget,
		&vUp);
	return m_matView;
}

D3DXMATRIX Camera::SetProjMatrix(float fFov, float fAspect, float fNear, float fFar)
{
	m_fFov = fFov; m_fNear = fNear;
	m_fAspect = fAspect; m_fFar = fFar;
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);

	return m_matProj;
}

void Camera::UpdateProjMatrix(UINT Width, UINT Height)
{
	m_fAspect = (float)Width / Height;
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
}