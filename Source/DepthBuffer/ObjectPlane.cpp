#include "ObjectPlane.h"

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
	m_VertexList[0].p = D3DXVECTOR3(0.0f, 0.0f, 0.0f);
	m_VertexList[0].c = D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f);

	m_VertexList[1].p = D3DXVECTOR3(0.0f, 1.0f, 0.0f);
	m_VertexList[1].c = D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f);

	m_VertexList[3].p = D3DXVECTOR3(1.0f, 0.0f, 0.0f);
	m_VertexList[3].c = D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f);

	D3DXVECTOR3 vDir0 = m_VertexList[1].p - m_VertexList[0].p;
	D3DXVECTOR3 vDir1 = m_VertexList[3].p - m_VertexList[0].p;
	D3DXVec3Normalize(&m_VertexList[2].p, &(vDir0 + vDir1));
	m_VertexList[2].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);

	D3DXVec3Normalize(&m_VertexList[5].p, &(-vDir0 * 1.0f));
	m_VertexList[5].c = D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f);
	D3DXVec3Normalize(&m_VertexList[6].p, &((vDir0 + vDir1) * -1.0f));
	m_VertexList[6].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
	D3DXVec3Normalize(&m_VertexList[7].p, &(-vDir1 * 1.0f));
	m_VertexList[7].c = D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);

	vDir0 = m_VertexList[3].p - m_VertexList[0].p;
	vDir1 = m_VertexList[5].p - m_VertexList[0].p;
	D3DXVec3Normalize(&m_VertexList[4].p, &(vDir0 + vDir1));
	m_VertexList[4].c = D3DXVECTOR4(1.0f, 0.0f, 1.0f, 1.0f);

	D3DXVec3Normalize(&m_VertexList[8].p, &((vDir0 + vDir1) * -1.0f));
	m_VertexList[8].c = D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f);
   		 
	m_indicies =
	{
		0, 1, 2,
		0, 2, 3,

		0, 3, 4,
		0, 4, 5,

		0, 5, 6,
		0, 6, 7,

		0, 7, 8,
		0, 8, 1,
	};

	DX::CreateVertexBuffer(pDevice, 
		(UINT)m_VertexList.size(), 
		sizeof(PC_VERTEX), 
		m_VertexList.data(), 
		&m_DxObject.m_pVertexBuffer);

	DX::CreateIndexBuffer(pDevice, 
		(UINT)m_indicies.size(), 
		sizeof(DWORD),
		m_indicies.data(),
		&m_DxObject.m_pIndexBuffer);

	DX::CreateConstantBuffer(pDevice, 
		1, 
		sizeof(CB_DATA), 
		&m_DxObject.m_pConstantBuffer, 
		&m_cbData, 
		false);


	m_iIndex = (UINT)m_indicies.size();
	m_iVertex = (UINT)m_VertexList.size();

	m_DxObject.m_pVertexShader.Attach(DX::LoadVertexShaderFile(pDevice, L"vs.vsh", &m_DxObject.m_pVSBlob));
	m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(pDevice, L"ps.psh");
	m_DxObject.m_pGeometryShader = DX::LoadGeometryShaderFile(pDevice, L"gs.gsh");

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DX::CreateInputLayout(pDevice, 
		(DWORD)m_DxObject.m_pVSBlob->GetBufferSize(), 
		m_DxObject.m_pVSBlob->GetBufferPointer(), 
		layout, 
		(UINT)std::size(layout), 
		&m_DxObject.m_pInputLayout);

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