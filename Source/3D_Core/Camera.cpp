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

	D3DXMATRIX mInvView;
	D3DXMatrixInverse(&mInvView, nullptr, &m_matView);
	D3DXVECTOR3 * pZBasis = (D3DXVECTOR3*)&mInvView._31;

	m_fCameraYawAngle = atan2f(pZBasis->x, pZBasis->z);
	float fLen = sqrtf(pZBasis->z * pZBasis->z + pZBasis->x * pZBasis->x);
	m_fCameraPitchAngle = -atan2f(pZBasis->y, fLen);

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
	//m_fSpeed -= g_fSecPerFrame * 1.0f;
	//if (m_fSpeed <= 1.0f) m_fSpeed = 1.0f;
	//else
	//{
	//	m_vPos += m_vLook * m_fSpeed * 0.5f;
	//}
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

	D3DXVec3Cross(&m_vUp, &m_vPos, &m_vSide);
	D3DXVec3Normalize(&m_vUp, &m_vUp);

	D3DXVec3Normalize(&m_vLook, &m_vLook);
	D3DXVec3Normalize(&m_vUpvector, &m_vUpvector);
	D3DXVec3Normalize(&m_vSide, &m_vSide);
}

bool Camera::Update(D3DXVECTOR4 vValue)
{
	D3DXMATRIX matRotation;
	D3DXQUATERNION qRotation;
	m_fCameraYawAngle += vValue.y;
	m_fCameraPitchAngle += vValue.x;
	D3DXQuaternionRotationYawPitchRoll(&qRotation, m_fCameraYawAngle, m_fCameraPitchAngle, vValue.z);

	m_vPos += m_vLook * vValue.w;

	D3DXMatrixAffineTransformation(&matRotation, 1.0f, nullptr, &qRotation, &m_vPos);

	D3DXMatrixInverse(&m_matView, nullptr, &matRotation);

	UpdateVector();
	return true;
}

void Camera::MoveLook(float fValue)
{
	m_vPos += m_vLook * fValue * m_fSpeed;
}

void Camera::MoveSide(float fValue)
{
	m_vPos += m_vSide * fValue * m_fSpeed;
}