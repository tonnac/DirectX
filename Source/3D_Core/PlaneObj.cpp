#include "PlaneObj.h"

HRESULT	PlaneObj::CreateVertexBuffer(ID3D11Device* pDevice)
{
	const int iNumCount = 4;
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

	m_vertexList[3].x = 0.5f, m_vertexList[3].y = -0.5f, m_vertexList[3].z = 0.5f;
	m_vertexList[3].u = 1.0f, m_vertexList[3].v = 1.0f;
	m_vertexList[3].r = 1.0f, m_vertexList[3].g = 1.0f, m_vertexList[3].b = 1.0f, m_vertexList[3].a = 1.0f;
	return Object::CreateVertexBuffer(pDevice);
}
HRESULT PlaneObj::CreateIndexBuffer(ID3D11Device* pDevice)
{
	m_indexList.push_back(0);
	m_indexList.push_back(1);
	m_indexList.push_back(2);
	m_indexList.push_back(3);
	m_indexList.push_back(2);
	m_indexList.push_back(1);

	return Object::CreateIndexBuffer(pDevice);
}
bool PlaneObj::Frame()
{
	return Object::Frame();
}
bool PlaneObj::PreRender(ID3D11DeviceContext* pContext)
{
	for (int i = 0; i < m_vertexList.size(); ++i)
	{
		PCT_VERTEX pe = m_vertexList[i];
		float S = sinf(g_fGameTimer);
		float C = cosf(g_fGameTimer);
		pe.x = m_vertexList[i].x * C + m_vertexList[i].y * S;
		pe.y = m_vertexList[i].y * C + m_vertexList[i].x * -S;

		m_vertexList[i].x = pe.x;
		m_vertexList[i].y = pe.y;
	}

	pContext->UpdateSubresource(m_pVertexBuffer, 0, NULL, &m_vertexList[0], 0, 0);
	return Object::PreRender(pContext);
}