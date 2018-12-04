#include "Shape.h"
#include <DirectXColors.h>

HRESULT SphereShape::CreateVertexData()
{
	constexpr float Radius = 1.0f;
	constexpr float X = 0.525731f;
	constexpr float Z = 0.850651f;

	D3DXVECTOR3 pos[12] =
	{
		D3DXVECTOR3(-X, 0.0f, Z),  D3DXVECTOR3(X, 0.0f, Z),
		D3DXVECTOR3(-X, 0.0f, -Z), D3DXVECTOR3(X, 0.0f, -Z),
		D3DXVECTOR3(0.0f, Z, X),   D3DXVECTOR3(0.0f, Z, -X),
		D3DXVECTOR3(0.0f, -Z, X),  D3DXVECTOR3(0.0f, -Z, -X),
		D3DXVECTOR3(Z, X, 0.0f),   D3DXVECTOR3(-Z, X, 0.0f),
		D3DXVECTOR3(Z, -X, 0.0f),  D3DXVECTOR3(-Z, -X, 0.0f)
	};

	DWORD k[60] =
	{
		1,4,0,  4,9,0,  4,5,9,  8,5,4,  1,8,4,
		1,10,8, 10,3,8, 8,3,5,  3,2,5,  3,7,2,
		3,10,7, 10,6,7, 6,11,7, 6,0,11, 6,1,0,
		10,1,6, 11,0,9, 2,11,9, 5,2,9,  11,2,7
	};

	MeshData meshData;
	meshData.m_vertices.resize(std::size(pos));
	meshData.m_indices.assign(&k[0], &k[60]);

	for (size_t i = 0; i < meshData.m_vertices.size(); ++i)
		meshData.m_vertices[i].p = pos[i];

	for (int i = 0; i < 3; ++i)
		Subdivide(meshData);

	for (size_t i = 0; i < meshData.m_vertices.size(); ++i)
	{
		D3DXVECTOR3 n;
		D3DXVec3Normalize(&n, &meshData.m_vertices[i].p);

		D3DXVECTOR3 p = n * Radius;

		meshData.m_vertices[i].p = p;
		meshData.m_vertices[i].n = n;
		meshData.m_vertices[i].c = D3DXVECTOR4(DirectX::Colors::Black);

		float theta = atan2f(meshData.m_vertices[i].p.z, meshData.m_vertices[i].p.x);

		if (theta < 0.0f)
			theta += (float)D3DX_PI * 2.0f;

		float phi = acosf(meshData.m_vertices[i].p.y / Radius);

		meshData.m_vertices[i].t.x = theta / (float)D3DX_PI * 2.0f;
		meshData.m_vertices[i].t.y = phi / (float)D3DX_PI;
	}
	m_VertexList.assign(meshData.m_vertices.begin(), meshData.m_vertices.end());
	m_IndexList.assign(meshData.m_indices.begin(), meshData.m_indices.end());

	m_DxObject.m_iNumVertex = (UINT)m_VertexList.size();
	m_DxObject.m_iNumIndex = (UINT)m_IndexList.size();
	m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);
	return S_OK;
}