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

	UpdateVector();
	return m_matView;
}

D3DXMATRIX Camera::SetProjMatrix(float fFov, float fAspect, float fNear, float fFar)
{
	m_fFov = fFov; m_fNear = fNear;
	m_fAspect = fAspect; m_fFar = fFar;
	D3DXMatrixPerspectiveFovLH(&m_matProj, fFov, fAspect, fNear, fFar);

	UpdateVector();
	return m_matProj;
}

void Camera::UpdateProjMatrix(UINT Width, UINT Height)
{
	m_fAspect = (float)Width / Height;
	D3DXMatrixPerspectiveFovLH(&m_matProj, m_fFov, m_fAspect, m_fNear, m_fFar);
	UpdateVector();
}

bool Camera::Frame()
{
	return true;
}

void Camera::UpdateVector()
{
	m_vLook.x = m_matView._13;
	m_vLook.y = m_matView._23;
	m_vLook.z = m_matView._33;

	m_vUpvector.x = m_matView._12;
	m_vUpvector.y = m_matView._22;
	m_vUpvector.z = m_matView._32;

	m_vSide.x = m_matView._11;
	m_vSide.y = m_matView._21;
	m_vSide.z = m_matView._31;

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vUpvector, &m_vUpvector);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
}

bool Camera::Update(D3DXVECTOR4 vValue)
{
	D3DXMATRIX matRotation;
	D3DXQUATERNION qRotation;
	D3DXQuaternionRotationYawPitchRoll(&qRotation, vValue.y, vValue.x, vValue.z);

	m_vPos += m_vLook * vValue.w;

	D3DXMatrixAffineTransformation(&matRotation, 1.0f, nullptr, &qRotation, &m_vPos);

	D3DXMatrixInverse(&m_matView, nullptr, &matRotation);

	UpdateVector();
	return true;
}