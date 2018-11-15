#pragma once
#include "ObjectStd.h"

class Shape
{
public:
	Shape();
	virtual ~Shape();

public:
	void Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName);

	virtual HRESULT CreateVertexData() { return S_OK; };
	virtual HRESULT CreateIndexData() { return S_OK; };

	virtual HRESULT CreateVertexBuffer();
	virtual HRESULT CreateIndexBuffer();
	virtual HRESULT CreateConstantBuffer();
	virtual HRESULT CreateInputLayout();
	virtual HRESULT LoadVertexShader(std::tstring szName);
	virtual HRESULT LoadPixelShader(std::tstring szName);
	virtual HRESULT LoadGeometryShader(std::tstring szName);
	virtual HRESULT LoadTextureShader(std::tstring szName);

	void SetMatrix(D3DXMATRIX* pWorld = nullptr, D3DXMATRIX* pView = nullptr, D3DXMATRIX* pProj = nullptr);
	void SetColor(D3DXVECTOR4 vColor);

	bool Init();
	bool Frame();

	bool PreRender(ID3D11DeviceContext* pContext);
	bool PostRender(ID3D11DeviceContext* pContext);
	bool Render(ID3D11DeviceContext* pContext);

	bool Release();

public:
	ID3D11Device* m_pDevice = nullptr;

	DX::DxObj m_DxObject;

	std::vector<PNCT_VERTEX> m_VertexList;
	std::vector<DWORD>		 m_IndexList;

	CB_DATA m_cbData;

	D3DXMATRIX m_matWorld;
	D3DXMATRIX m_matView;
	D3DXMATRIX m_matProj;

	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;
	D3DXVECTOR3 m_vUp;

	UINT m_iVertexSize;

	D3D_PRIMITIVE_TOPOLOGY m_Primitive;
};

class BoxShape : public Shape
{
public:
	BoxShape();
	virtual ~BoxShape();

public:
	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;
};

class LineShape : public Shape
{
public:
	LineShape();
	virtual ~LineShape();
public:
//	bool Draw(ID3D11DeviceContext* pContext, )

public:
//	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;
};

class PlaneShape : public Shape
{
public:
	PlaneShape();
	virtual ~PlaneShape();

public:
	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;
};

class DirectionShape : public Shape
{
public:
	DirectionShape();
	virtual ~DirectionShape();

public:
	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;
};