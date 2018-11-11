#include "Sphere.h"
#include <DirectXColors.h>

Sphere::Sphere()
{

}
Sphere::~Sphere()
{

}

bool Sphere::Init()
{
	return true;
}
bool Sphere::Set(ID3D11Device* pDevice, float radius, int slicecount, int stackcount)
{
	float phiStep = (float)D3DX_PI / (stackcount - 1);
	float thetaStep = (float)(2 * D3DX_PI) / slicecount;

	PC_VERTEX topVertex = { D3DXVECTOR3(0.0f, radius, 0.0f)};
	PC_VERTEX botVertex = { D3DXVECTOR3(0.0f, -radius, 0.0f)};
	D3DXVec3Normalize(&topVertex.n, &topVertex.p);
	D3DXVec3Normalize(&botVertex.n, &botVertex.p);

	m_VertexList.push_back(topVertex);

	for (int i = 0; i < stackcount - 1; ++i)
	{
		float phi = i * phiStep;
		for (int j = 0; j <= slicecount; ++j)
		{
			PC_VERTEX v;

			float theta = j * thetaStep;

			float x = radius * sinf(phi) * cosf(theta);
			float y = radius * cosf(phi);
			float z = radius * sinf(phi) * sinf(theta);

			v.p = { x,y,z };
			D3DXVec3Normalize(&v.n, &v.p);

			m_VertexList.push_back(v);
		}
	}

	m_VertexList.push_back(botVertex);

	for (int i = 0; i < slicecount; ++i)
	{
		m_indicies.push_back(0);
		m_indicies.push_back(i + 2);
		m_indicies.push_back(i + 1);
	}

	int baseIndex = 1;
	int ringVertexCount = slicecount + 1;

	for (int i = 0; i < stackcount - 2; ++i)
	{
		for (int j = 0; j < slicecount; ++j)
		{
			m_indicies.push_back(baseIndex + i * ringVertexCount + j);
			m_indicies.push_back(baseIndex + i * ringVertexCount + j + 1);
			m_indicies.push_back(baseIndex + (i + 1) * ringVertexCount + j);

			m_indicies.push_back(baseIndex + i * ringVertexCount + j + 1);
			m_indicies.push_back(baseIndex + (i + 1) * ringVertexCount + j + 1);
			m_indicies.push_back(baseIndex + (i + 1) * ringVertexCount + j);
		}
	}

	int southPoleindex = m_VertexList.size() - 1;
	baseIndex = southPoleindex - ringVertexCount;

	for (int i = 0; i < slicecount; ++i)
	{
		m_indicies.push_back(southPoleindex);
		m_indicies.push_back(baseIndex + i);
		m_indicies.push_back(baseIndex + i + 1);
	}

	m_iIndex = (UINT)m_indicies.size();
	m_iVertex = (UINT)m_VertexList.size();

	DX::CreateVertexBuffer(pDevice, m_iVertex, sizeof(PC_VERTEX), m_VertexList.data(), &m_DxObject.m_pVertexBuffer);
	DX::CreateIndexBuffer(pDevice, m_iIndex, sizeof(DWORD), m_indicies.data(), &m_DxObject.m_pIndexBuffer);
	DX::CreateConstantBuffer(pDevice, 1, sizeof(CB_DATA), &m_DxObject.m_pConstantBuffer);

	m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(pDevice, L"ps.psh");
	m_DxObject.m_pVertexShader = DX::LoadVertexShaderFile(pDevice, L"vs.vsh", &m_DxObject.m_pVSBlob);

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DX::CreateInputLayout(pDevice, (DWORD)m_DxObject.m_pVSBlob->GetBufferSize(),
		m_DxObject.m_pVSBlob->GetBufferPointer(),
		layout,
		(UINT)std::size(layout),
		&m_DxObject.m_pInputLayout);


	return true;
}

bool Sphere::Frame()
{
	m_cbData.fTime = g_fGameTimer;
	return true;
}

bool Sphere::Render(ID3D11DeviceContext* pContext)
{
	pContext->UpdateSubresource(m_DxObject.m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
	m_DxObject.Render(pContext, sizeof(PC_VERTEX), m_iIndex);
	return true;
}

bool Sphere::Release()
{
	m_DxObject.Release();
	return true;
}
