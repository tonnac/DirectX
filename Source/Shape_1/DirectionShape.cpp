#include "Shape.h"
#include <DirectXColors.h>

DirectionShape::DirectionShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_LINELIST;
}

DirectionShape::~DirectionShape()
{

}

HRESULT	DirectionShape::CreateInputLayout()
{
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR", 0, DXGI_FORMAT_R32G32B32A32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DX::CreateInputLayout(m_pDevice, (DWORD)m_DxObject.m_pVSBlob.Get()->GetBufferSize(),
		m_DxObject.m_pVSBlob.Get()->GetBufferPointer(), layout, (UINT)std::size(layout), &m_DxObject.m_pInputLayout);

	return S_OK;
}

HRESULT	DirectionShape::CreateVertexData()
{
	m_LineVertexList =
	{
		PC_VERTEX({D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR4(DirectX::Colors::Red)}),
		PC_VERTEX({D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR4(DirectX::Colors::Green)}),
		PC_VERTEX({D3DXVECTOR3(0.0f,0.0f,0.0f), D3DXVECTOR4(DirectX::Colors::Blue)}),
		PC_VERTEX({D3DXVECTOR3(1000.0f,0.0f,0.0f), D3DXVECTOR4(DirectX::Colors::Red)}),
		PC_VERTEX({D3DXVECTOR3(0.0f,1000.0f,0.0f), D3DXVECTOR4(DirectX::Colors::Green)}),
		PC_VERTEX({D3DXVECTOR3(0.0f,0.0f,1000.0f), D3DXVECTOR4(DirectX::Colors::Blue)}),
	};

	m_DxObject.m_iVertexSize = sizeof(PC_VERTEX);
	m_DxObject.m_iNumVertex = (UINT)std::size(m_LineVertexList);
	return S_OK;
}

HRESULT DirectionShape::CreateIndexData()
{
	m_IndexList =
	{ 0, 3, 1, 4, 2, 5 };

	m_DxObject.m_iNumIndex = (UINT)std::size(m_IndexList);
	return S_OK;
}

HRESULT DirectionShape::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	DX::CreateVertexBuffer(m_pDevice,
		(UINT)m_LineVertexList.size(),
		m_DxObject.m_iVertexSize,
		m_LineVertexList.data(),
		&m_DxObject.m_pVertexBuffer);

	return hr;
}

HRESULT DirectionShape::LoadVertexShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	m_DxObject.m_pVertexShader.Attach(DX::LoadVertexShaderFile(m_pDevice, szName.c_str(), &m_DxObject.m_pVSBlob, "VS_PC"));
	return hr;
}

HRESULT DirectionShape::LoadPixelShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(m_pDevice, szName.c_str(), nullptr, "PSLine");
	return hr;
}

