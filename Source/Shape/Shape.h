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

	UINT m_iVertexSize;
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