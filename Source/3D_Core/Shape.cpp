#include "Shape.h"
#include <DirectXColors.h>

D3DXMATRIX Identity()
{
	D3DXMATRIX retMat;
	D3DXMatrixIdentity(&retMat);
	return retMat;
}

Shape::Shape()
{
	D3DXMatrixIdentity(&m_matWorld);
	D3DXMatrixIdentity(&m_matView);
	D3DXMatrixIdentity(&m_matProj);
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;

	m_vLook = { m_matWorld._11, m_matWorld._12 , m_matWorld._13 };
	m_vSide = { m_matWorld._21, m_matWorld._22 , m_matWorld._23 };
	m_vUp = { m_matWorld._31, m_matWorld._32 , m_matWorld._33 };
	m_vPosition = { m_matWorld._41, m_matWorld._42, m_matWorld._43 };
}

Shape::~Shape()
{

}

void Shape::Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName)
{
	m_pDevice = pDevice;
	CreateVertexData();
	CreateIndexData();
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	LoadVertexShader(szShaderName);
	LoadPixelShader(szShaderName);
	//	LoadGeometryShader(L"gs.psh");
	CreateInputLayout();
	if (!szTextureName.empty())
	{
		LoadTextureShader(szTextureName);
	}
}

HRESULT Shape::CreateVertexBuffer()
{
	HRESULT hr = S_OK;

	DX::CreateVertexBuffer(m_pDevice,
		(UINT)m_VertexList.size(),
		m_DxObject.m_iVertexSize,
		m_VertexList.data(),
		&m_DxObject.m_pVertexBuffer);

	return hr;
}

HRESULT Shape::CreateIndexBuffer()
{
	HRESULT hr = S_OK;
	DX::CreateIndexBuffer(m_pDevice,
		(UINT)m_IndexList.size(),
		sizeof(DWORD),
		m_IndexList.data(),
		&m_DxObject.m_pIndexBuffer);
	return hr;
}

HRESULT Shape::CreateConstantBuffer()
{
	HRESULT hr = S_OK;
	DX::CreateConstantBuffer(m_pDevice,
		1,
		sizeof(CB_DATA),
		&m_DxObject.m_pConstantBuffer,
		&m_cbData,
		false);
	return hr;
}

HRESULT Shape::CreateInputLayout()
{
	HRESULT hr = S_OK;

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 0	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"NORMAL"	, 0, DXGI_FORMAT_R32G32B32_FLOAT	, 0, 12	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"COLOR"	, 0, DXGI_FORMAT_R32G32B32A32_FLOAT	, 0, 24	, D3D11_INPUT_PER_VERTEX_DATA, 0},
		{"TEXCOORD"	, 0, DXGI_FORMAT_R32G32_FLOAT	, 0, 40	, D3D11_INPUT_PER_VERTEX_DATA, 0}
	};

	DX::CreateInputLayout(m_pDevice,
		(DWORD)m_DxObject.m_pVSBlob->GetBufferSize(),
		m_DxObject.m_pVSBlob->GetBufferPointer(),
		layout,
		(UINT)std::size(layout),
		&m_DxObject.m_pInputLayout);

	return hr;
}

HRESULT Shape::LoadVertexShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	m_DxObject.m_pVertexShader.Attach(DX::LoadVertexShaderFile(m_pDevice, szName.c_str(), &m_DxObject.m_pVSBlob));
	return hr;
}

HRESULT Shape::LoadPixelShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(m_pDevice, szName.c_str());
	return hr;
}

HRESULT Shape::LoadGeometryShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	m_DxObject.m_pGeometryShader = DX::LoadGeometryShaderFile(m_pDevice, szName.c_str());
	return hr;
}

HRESULT Shape::LoadTextureShader(std::tstring szName)
{
	HRESULT hr = S_OK;

	m_DxObject.m_pTextureSRV = DX::CreateShaderResourceView(m_pDevice, szName);

	return hr;
}

void Shape::SetMatrix(D3DXMATRIX* pWorld, D3DXMATRIX* pView, D3DXMATRIX* pProj)
{
	if (pWorld != nullptr)
	{
		m_matWorld = *pWorld;
	}
	else
	{
		 D3DXMatrixIdentity(&m_matWorld);
	}
	if (pView != nullptr)
	{
		m_matView = *pView;
	}
	if (pProj != nullptr)
	{
		m_matProj = *pProj;
	}

	D3DXMATRIX invworld;
	D3DXMatrixInverse(&invworld, nullptr, &m_matWorld);
	D3DXMatrixTranspose(&invworld, &invworld);

	m_vLook = { m_matWorld._11, m_matWorld._12 , m_matWorld._13 };
	m_vSide = { m_matWorld._21, m_matWorld._22 , m_matWorld._23 };
	m_vUp = { m_matWorld._31, m_matWorld._32 , m_matWorld._33 };
	m_vPosition = { m_matWorld._41, m_matWorld._42, m_matWorld._43 };

	D3DXMatrixTranspose(&m_cbData.matWorld, &m_matWorld);
	D3DXMatrixTranspose(&m_cbData.matView, &m_matView);
	D3DXMatrixTranspose(&m_cbData.matProj, &m_matProj);
	D3DXMatrixTranspose(&m_cbData.matInvWorld, &invworld);
}

void Shape::SetColor(D3DXVECTOR4 vColor)
{
	m_cbData.vColor = vColor;
}

bool Shape::Init()
{
	return true;
}

bool Shape::Frame()
{
	return true;
}

bool Shape::PreRender(ID3D11DeviceContext* pContext)
{
	pContext->IASetPrimitiveTopology(m_Primitive);
	pContext->UpdateSubresource(m_DxObject.m_pConstantBuffer.Get(), 0, nullptr, &m_cbData, 0, 0);
	return m_DxObject.PreRender(pContext, m_DxObject.m_iVertexSize);
}

bool Shape::PostRender(ID3D11DeviceContext* pContext)
{
	m_DxObject.PostRender(pContext, m_DxObject.m_iVertexSize, m_DxObject.m_iNumIndex);
	return true;
}

bool Shape::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}

bool Shape::Release()
{
	m_DxObject.Release();
	return true;
}

BoxShape::BoxShape(bool isDice) : mIsDice(isDice)
{

}

BoxShape::~BoxShape()
{

}

HRESULT BoxShape::CreateVertexData()
{
	HRESULT hr = S_OK;

	m_VertexList.resize(24);

	m_VertexList[0] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[1] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[2] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[3] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, 0.0f, -1.0f), D3DXVECTOR4(1.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));
	// µÞ¸é
	m_VertexList[4] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 0.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[5] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[6] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[7] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, 0.0f, 1.0f), D3DXVECTOR4(0.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// ¿À¸¥ÂÊ
	m_VertexList[8] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[9] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[10] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[11] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(1.0f, 0.0f, 0.0f), D3DXVECTOR4(0.0f, 0.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// ¿ÞÂÊ
	m_VertexList[12] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[13] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[14] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[15] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(-1.0f, 0.0f, 0.0f), D3DXVECTOR4(1.0f, 1.0f, 0.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// À­¸é
	m_VertexList[16] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[17] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, 1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[18] = PNCT_VERTEX(D3DXVECTOR3(1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[19] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, 1.0f, -1.0f), D3DXVECTOR3(0.0f, 1.0f, 0.0f), D3DXVECTOR4(1.0f, 0.5f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	// ¾Æ·§¸é
	m_VertexList[20] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 0.0f));
	m_VertexList[21] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, -1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 0.0f));
	m_VertexList[22] = PNCT_VERTEX(D3DXVECTOR3(1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(1.0f, 1.0f));
	m_VertexList[23] = PNCT_VERTEX(D3DXVECTOR3(-1.0f, -1.0f, 1.0f), D3DXVECTOR3(0.0f, -1.0f, 0.0f), D3DXVECTOR4(0.0f, 1.0f, 1.0f, 1.0f), D3DXVECTOR2(0.0f, 1.0f));

	if (mIsDice)
		DiceTex();

	m_DxObject.m_iNumVertex = (UINT)m_VertexList.size();
	m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);

	return hr;
}

HRESULT BoxShape::CreateIndexData()
{
	HRESULT hr = S_OK;
	m_IndexList.resize(36);

	//Front
	m_IndexList[0] = 0; m_IndexList[1] = 1; m_IndexList[2] = 2;
	m_IndexList[3] = 0; m_IndexList[4] = 2; m_IndexList[5] = 3;

	//Back
	m_IndexList[6] = 4; m_IndexList[7] = 5; m_IndexList[8] = 6;
	m_IndexList[9] = 4; m_IndexList[10] = 6; m_IndexList[11] = 7;

	//Top
	m_IndexList[12] = 8; m_IndexList[13] = 9; m_IndexList[14] = 10;
	m_IndexList[15] = 8; m_IndexList[16] = 10; m_IndexList[17] = 11;

	//Bottom
	m_IndexList[18] = 12; m_IndexList[19] = 13; m_IndexList[20] = 14;
	m_IndexList[21] = 12; m_IndexList[22] = 14; m_IndexList[23] = 15;

	//Left
	m_IndexList[24] = 16; m_IndexList[25] = 17; m_IndexList[26] = 18;
	m_IndexList[27] = 16; m_IndexList[28] = 18; m_IndexList[29] = 19;

	//Right
	m_IndexList[30] = 20; m_IndexList[31] = 21; m_IndexList[32] = 22;
	m_IndexList[33] = 20; m_IndexList[34] = 22; m_IndexList[35] = 23;

	m_DxObject.m_iNumIndex = (UINT)m_IndexList.size();
	return hr;
}

void BoxShape::DiceTex()
{
	m_VertexList[0].t = { 0.76f, 0.66f };
	m_VertexList[1].t = { 0.76f, 0.34f };
	m_VertexList[2].t = { 1.0f, 0.34f };
	m_VertexList[3].t = { 1.0f, 0.66f };

	//6
	m_VertexList[4].t = { 0.5f, 0.33f };
	m_VertexList[5].t = { 0.25f, 0.33f };
	m_VertexList[6].t = { 0.25f, 0.0f };
	m_VertexList[7].t = { 0.5f, 0.0f };

	//2
	m_VertexList[8].t = { 0.51f, 0.66f };
	m_VertexList[9].t = { 0.51f, 0.34f };
	m_VertexList[10].t = { 0.75f, 0.34f };
	m_VertexList[11].t = { 0.75f, 0.66f };

	//3
	m_VertexList[12].t = { 0.5f, 0.66f };
	m_VertexList[13].t = { 0.25f, 0.66f };
	m_VertexList[14].t = { 0.25f, 0.33f };
	m_VertexList[15].t = { 0.5f, 0.33f };

	//5
	m_VertexList[16].t = { 0.26f, 1.0f };
	m_VertexList[17].t = { 0.26f, 0.67f };
	m_VertexList[18].t = { 0.5f, 0.67f };
	m_VertexList[19].t = { 0.5f, 1.0f };

	//4

	m_VertexList[20].t = { 0.0f, 0.66f };
	m_VertexList[21].t = { 0.0f, 0.33f };
	m_VertexList[22].t = { 0.25f, 0.33f };
	m_VertexList[23].t = { 0.25f, 0.66f };
}