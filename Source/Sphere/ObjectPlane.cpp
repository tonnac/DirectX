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
	m_VertexList =
	{
		PC_VERTEX({ D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR4(DirectX::Colors::Aquamarine)	}),
		PC_VERTEX({ D3DXVECTOR3(-1.0f, +1.0f, -1.0f), D3DXVECTOR4(DirectX::Colors::Cyan)		}),
		PC_VERTEX({ D3DXVECTOR3(+1.0f, +1.0f, -1.0f), D3DXVECTOR4(DirectX::Colors::Red)			}),
		PC_VERTEX({ D3DXVECTOR3(+1.0f, -1.0f, -1.0f), D3DXVECTOR4(DirectX::Colors::Sienna)		}),
		PC_VERTEX({ D3DXVECTOR3(-1.0f, -1.0f, +1.0f), D3DXVECTOR4(DirectX::Colors::Gold)		}),
		PC_VERTEX({ D3DXVECTOR3(-1.0f, +1.0f, +1.0f), D3DXVECTOR4(DirectX::Colors::Magenta)		}),
		PC_VERTEX({ D3DXVECTOR3(+1.0f, +1.0f, +1.0f), D3DXVECTOR4(DirectX::Colors::LemonChiffon)}),
		PC_VERTEX({ D3DXVECTOR3(+1.0f, -1.0f, +1.0f), D3DXVECTOR4(DirectX::Colors::Azure)		}),
	};
   		 
	m_indicies =
	{
		0, 1, 2,
		0, 2, 3,

		4, 5, 1,
		4, 1, 0,

		4, 6, 5,
		4, 7, 6,

		3, 2, 6,
		3, 6, 7,

		1, 5, 6,
		1, 6, 2,

		4, 0, 3,
		4, 3, 7
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