#include "ModelView.h"

ModelView::ModelView()
{
	D3DXMatrixIdentity(&m_matModelRot);
	D3DXMatrixIdentity(&m_matModelLast);
}

bool ModelView::Update(D3DXVECTOR4 vValue)
{
	Camera::Update(vValue);
	D3DXMATRIX mModelRot, mModelLastRotInv, mInvView;
	mModelRot = m_worldArcball.getRotationMat();

	D3DXMatrixInverse(&mModelLastRotInv, nullptr, &m_matModelLast);
	D3DXMatrixInverse(&mInvView, nullptr, &m_matView);

	mInvView._41 = mInvView._42 = mInvView._43 = 0;

	m_matModelRot = m_matModelRot * m_matView * mModelLastRotInv * mModelRot *  mInvView;

	m_matModelRot._41 = m_matModelRot._42 = m_matModelRot._43 = 0;

	m_matModelLast = mModelRot;
	return true;
}

LRESULT ModelView::MsgProc(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam)
{
	short iMouseX = LOWORD(lparam);
	short iMouseY = HIWORD(lparam);

	switch (msg)
	{

	case WM_LBUTTONDOWN:
	case WM_LBUTTONDBLCLK:
		SetCapture(hwnd);
		m_worldArcball.OnBegin(iMouseX, iMouseY);
		return 1;

	case WM_LBUTTONUP:
		ReleaseCapture();
		m_worldArcball.OnEnd();
		return 1;

	case WM_MOUSEMOVE:
		if (MK_LBUTTON & wparam)
		{
			m_worldArcball.OnMove(iMouseX, iMouseY);
		}
		return 1;
		
	}
	return 1;
}