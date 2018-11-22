#pragma once
#include "ObjectStd.h"

D3DXMATRIX Identity();

struct SRTMatrix
{
	float		fSpin = 0.0f;
	float		fOrbit = 0.0f;
	D3DXMATRIX	matScale;
	D3DXMATRIX	matSpinRotation;
	D3DXMATRIX	matOrbitRotation;
	D3DXMATRIX	matTranslate;
	D3DXMATRIX	matWorld;
	SRTMatrix()
	{
		matScale = Identity();
		matSpinRotation = Identity();
		matOrbitRotation = Identity();
		matTranslate = Identity();
		matWorld = Identity();
	}
};

class Shape
{
public:
	Shape();
	virtual ~Shape();

public:
	void Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName = std::tstring());

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
	virtual bool Render(ID3D11DeviceContext* pContext);

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

	SRTMatrix  m_SRTMat;

	D3DXVECTOR3 m_vPosition;
	D3DXVECTOR3 m_vLook;
	D3DXVECTOR3 m_vSide;
	D3DXVECTOR3 m_vUp;

	D3D_PRIMITIVE_TOPOLOGY m_Primitive;
};

class BoxShape : public Shape
{
public:
	BoxShape(bool isDice = false);
	virtual ~BoxShape();

public:
	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;

	void DiceTex();
public:
	bool mIsDice = false;
};

class LineShape : public Shape
{
public:
	LineShape();
	virtual ~LineShape();
public:
	HRESULT	CreateInputLayout()override;
	HRESULT	CreateVertexData()override;
	HRESULT	CreateIndexData()override;
	HRESULT	CreateVertexBuffer()override;
	HRESULT LoadVertexShader(std::tstring szName)override;
	HRESULT LoadPixelShader(std::tstring szName)override;
	bool	Draw(ID3D11DeviceContext* pContext, D3DXVECTOR3 vStart, D3DXVECTOR3 vEnd, D3DXVECTOR4 vColor);

public:
	std::array<PC_VERTEX, 2> m_LineVertexList;
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
	HRESULT	CreateInputLayout()override;
	HRESULT	CreateVertexData()override;
	HRESULT	CreateIndexData()override;
	HRESULT	CreateVertexBuffer()override;
	HRESULT LoadVertexShader(std::tstring szName)override;
	HRESULT LoadPixelShader(std::tstring szName)override;

public:
	std::array<PC_VERTEX, 6> m_LineVertexList;
};

class DiceShape : public BoxShape
{
public:
	HRESULT	CreateVertexData()override;
};