#include "TriangleObj.h"

HRESULT	TriangleObj::CreateVertexBuffer(ID3D11Device* pDevice)
{
	const int iNumCount = 3;
	m_vertexList.resize(iNumCount);
	m_vertexList[0].x = -0.5f, m_vertexList[0].y = 0.5f, m_vertexList[0].z = 0.5f;
	m_vertexList[0].u = 0.0f, m_vertexList[0].v = 0.0f;
	m_vertexList[0].r = 1.0f, m_vertexList[0].g = 0.0f, m_vertexList[0].b = 0.0f, m_vertexList[0].a = 0.0f;

	m_vertexList[1].x = 0.5f, m_vertexList[1].y = 0.5f, m_vertexList[1].z = 0.5f;
	m_vertexList[1].u = 1.0f, m_vertexList[1].v = 0.0f;
	m_vertexList[1].r = 0.0f, m_vertexList[1].g = 0.0f, m_vertexList[1].b = 1.0f, m_vertexList[1].a = 0.0f;

	m_vertexList[2].x = -0.5f, m_vertexList[2].y = -0.5f, m_vertexList[2].z = 0.5f;
	m_vertexList[2].u = 0.0f, m_vertexList[2].v = 1.0f;
	m_vertexList[2].r = 0.0f, m_vertexList[2].g = 1.0f, m_vertexList[2].b = 0.0f, m_vertexList[2].a = 0.0f;

	return Object::CreateVertexBuffer(pDevice);
}
HRESULT	TriangleObj::CreateIndexBuffer(ID3D11Device* pDevice)
{
	return S_OK;
}
bool TriangleObj::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->Draw(3, 0);
	return true;
}