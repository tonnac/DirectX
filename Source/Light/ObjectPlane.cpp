#include "ObjectPlane.h"
#include <DirectXColors.h>



ObjectPlane::ObjectPlane()
{

}
ObjectPlane::~ObjectPlane()
{

}

bool ObjectPlane::Init()
{
	m_cbData.fTime = 1.0f;
	return true;
}
bool ObjectPlane::Set(ID3D11Device* pDevice)
{
	return true;
}
bool ObjectPlane::Frame()
{
	m_cbData.fTime = cosf(g_fGameTimer) * 0.5f + 0.5f;
	return true;
}
bool ObjectPlane::Render(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_DxObject.m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
	m_DxObject.Render(pContext, sizeof(PC_VERTEX), m_iIndex);
	return true;
}
bool ObjectPlane::Release()
{
	m_DxObject.Release();
	return true;
}