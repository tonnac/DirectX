#include "Frustum.h"
#include "DirectXColors.h"

using namespace DirectX;

bool Frustum::Init(ID3D11Device * pDevice)
{
	m_VertexList.resize(24);

	m_VertexList[0] = PNCT_VERTEX(m_vFrustum[1], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(Colors::DeepSkyBlue), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(m_vFrustum[2], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(Colors::DeepSkyBlue), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(m_vFrustum[3], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(Colors::DeepSkyBlue), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(m_vFrustum[0], D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(Colors::DeepSkyBlue), D3DXVECTOR2(0.0f, 1.0f));

	m_VertexList[4] = PNCT_VERTEX(m_vFrustum[6], D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(Colors::Cornsilk), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(m_vFrustum[5], D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(Colors::Cornsilk), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(m_vFrustum[4], D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(Colors::Cornsilk), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(m_vFrustum[7], D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(Colors::Cornsilk), D3DXVECTOR2(0.0f, 1.0f));

	m_VertexList[8] = PNCT_VERTEX(m_vFrustum[5], D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::MediumTurquoise), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(m_vFrustum[1], D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::MediumTurquoise), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(m_vFrustum[0], D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::MediumTurquoise), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(m_vFrustum[4], D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::MediumTurquoise), D3DXVECTOR2(0.0f, 1.0f));

	m_VertexList[12] = PNCT_VERTEX(m_vFrustum[2], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::Olive), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(m_vFrustum[6], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::Olive), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(m_vFrustum[7], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::Olive), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(m_vFrustum[3], D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(Colors::Olive), D3DXVECTOR2(0.0f, 1.0f));

	m_VertexList[16] = PNCT_VERTEX(m_vFrustum[5], D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(Colors::Yellow), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(m_vFrustum[6], D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(Colors::Yellow), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(m_vFrustum[2], D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(Colors::Yellow), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(m_vFrustum[1], D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(Colors::Yellow), D3DXVECTOR2(0.0f, 1.0f));

	m_VertexList[20] = PNCT_VERTEX(m_vFrustum[0], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(Colors::Pink), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(m_vFrustum[3], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(Colors::Pink), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(m_vFrustum[7], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(Colors::Pink), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(m_vFrustum[4], D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(Colors::Pink), D3DXVECTOR2(0.0f, 1.0f));

	m_box.Create(pDevice, L"frustum.hlsl");
	return true;
}

bool Frustum::CreateFrustum()
{
	D3DXMATRIX matInvViewProj;
	D3DXMatrixMultiply(&matInvViewProj, m_View, m_Proj);

	D3DXMatrixInverse(&matInvViewProj, nullptr, &matInvViewProj);

	m_vFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, +0.0f);
	m_vFrustum[1] = D3DXVECTOR3(-1.0f, +1.0f, +0.0f);
	m_vFrustum[2] = D3DXVECTOR3(+1.0f, +1.0f, +0.0f);
	m_vFrustum[3] = D3DXVECTOR3(+1.0f, -1.0f, +0.0f);

	m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	m_vFrustum[5] = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	m_vFrustum[6] = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	m_vFrustum[7] = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);

	for (int iVertex = 0; iVertex < 8; ++iVertex)
	{
		D3DXVec3TransformCoord(&m_vFrustum[iVertex], &m_vFrustum[iVertex], &matInvViewProj);
	}

	m_Plane[0].CreatePlane(m_vFrustum[0], m_vFrustum[1], m_vFrustum[2]);
	m_Plane[1].CreatePlane(m_vFrustum[6], m_vFrustum[5], m_vFrustum[4]);

	m_Plane[2].CreatePlane(m_vFrustum[2], m_vFrustum[6], m_vFrustum[7]);
	m_Plane[3].CreatePlane(m_vFrustum[5], m_vFrustum[1], m_vFrustum[0]);

	m_Plane[4].CreatePlane(m_vFrustum[1], m_vFrustum[5], m_vFrustum[6]);
	m_Plane[5].CreatePlane(m_vFrustum[0], m_vFrustum[3], m_vFrustum[7]);

	return true;
}

bool Frustum::ClassifyPoint(D3DXVECTOR3 v)
{
	for (int i = 0; i < 6; ++i)
	{
		D3DXVECTOR3 n = { m_Plane[i].a, m_Plane[i].b, m_Plane[i].c };
		float fDistance = D3DXVec3Dot(&n, &v) + m_Plane[i].d;
		
		if (fDistance > 0)
		{
			return false;
		}
	}

	return true;
}

bool Frustum::ClassifySphere(D3DXVECTOR3 v)
{
	return true;
}

bool Frustum::Render(ID3D11DeviceContext* pContext)
{

	//m_vFrustum[0] = D3DXVECTOR3(-1.0f, -1.0f, +0.0f);
	//m_vFrustum[1] = D3DXVECTOR3(-1.0f, +1.0f, +0.0f);
	//m_vFrustum[2] = D3DXVECTOR3(+1.0f, +1.0f, +0.0f);
	//m_vFrustum[3] = D3DXVECTOR3(+1.0f, -1.0f, +0.0f);

	//m_vFrustum[4] = D3DXVECTOR3(-1.0f, -1.0f, +1.0f);
	//m_vFrustum[5] = D3DXVECTOR3(-1.0f, +1.0f, +1.0f);
	//m_vFrustum[6] = D3DXVECTOR3(+1.0f, +1.0f, +1.0f);
	//m_vFrustum[7] = D3DXVECTOR3(+1.0f, -1.0f, +1.0f);

	m_VertexList[0].p = m_vFrustum[0];
	m_VertexList[1].p = m_vFrustum[1];
	m_VertexList[2].p = m_vFrustum[2];
	m_VertexList[3].p = m_vFrustum[3];

	m_VertexList[4].p = m_vFrustum[4];
	m_VertexList[5].p = m_vFrustum[7];
	m_VertexList[6].p = m_vFrustum[6];
	m_VertexList[7].p = m_vFrustum[5];

	m_VertexList[8].p = m_vFrustum[1];
	m_VertexList[9].p = m_vFrustum[5];
	m_VertexList[10].p = m_vFrustum[6];
	m_VertexList[11].p = m_vFrustum[2];

	m_VertexList[12].p = m_vFrustum[0];
	m_VertexList[13].p = m_vFrustum[3];
	m_VertexList[14].p = m_vFrustum[4];
	m_VertexList[15].p = m_vFrustum[7];

	m_VertexList[16].p = m_vFrustum[4];
	m_VertexList[17].p = m_vFrustum[5];
	m_VertexList[18].p = m_vFrustum[1];
	m_VertexList[19].p = m_vFrustum[0];

	m_VertexList[20].p = m_vFrustum[3];
	m_VertexList[21].p = m_vFrustum[2];
	m_VertexList[22].p = m_vFrustum[6];
	m_VertexList[23].p = m_vFrustum[7];

	pContext->UpdateSubresource(m_box.m_DxObject.m_pVertexBuffer.Get(), 0, nullptr, m_VertexList.data(), 0, 0);
	return m_box.Render(pContext);
}

void Frustum::SetMatrix(D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	m_View = pView;
	m_Proj = pProj;
}