#include "ObjectPlane.h"

ObjectPlane::ObjectPlane()
{

}
ObjectPlane::~ObjectPlane()
{

}

bool ObjectPlane::Init()
{
	return true;
}
bool ObjectPlane::Set(ID3D11Device* pDevice)
{
	Vector3 v(0.0f, 0.0f, 0.0f);
	Vector3 v2(0.0f, 1.0f, 0.0f);
	Vector3 v4(1.0f, 0.0f, 0.0f);

	Vector3 v0 = v4 * -1.0f;
	Vector3 v1 = (v0 + v2).Normalize();
	Vector3 v3 = (v2 + v4).Normalize();

	v0.Normal();
	v1.Normal();
	v3.Normal();

	float fDot = v2 | v4;
	Vector3 vCross = v2 ^ v4;

	m_VertexList =
	{
		m_VertexList[0] = {v , Vector4(1.0f, 0.0f, 0.0f, 1.0f)},
		m_VertexList[1] = {v0, Vector4(0.0f, 1.0f, 0.0f, 1.0f)},
		m_VertexList[2] = {v1, Vector4(0.0f, 0.0f, 1.0f, 1.0f)},
		m_VertexList[3] = {v2, Vector4(1.0f, 1.0f, 1.0f, 1.0f)},

		m_VertexList[4] = {v3, Vector4(1.0f, 0.0f, 0.0f, 1.0f)},
		m_VertexList[5] = {v4, Vector4(0.0f, 1.0f, 0.0f, 1.0f)},
	};

	m_indicies =
	{
		0, 1, 2,
		0, 2, 3,

		0, 3, 4,
		0, 4, 5
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
		nullptr, 
		true);

	m_iIndex = (UINT)m_indicies.size();
	m_iVertex = (UINT)m_VertexList.size();

	m_DxObject.m_pVertexShader = DX::LoadVertexShaderFile(pDevice, L"vs.vsh", &m_DxObject.m_pVSBlob);
	m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(pDevice, L"ps.psh");
	
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
	return true;
}
bool ObjectPlane::Render(ID3D11DeviceContext* pContext)
{
	m_DxObject.Render(pContext, sizeof(PC_VERTEX), m_iIndex);
	return true;
}
bool ObjectPlane::Release()
{
	m_DxObject.Release();
	return true;
}