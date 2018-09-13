#pragma once
#include "Core.h"
#include "Texture.h"
#include "Shader.h"

#define GPU
//#define CPU

struct PCT_VERTEX // pnct
{
	float x, y, z;
	float r, g, b, a;
	float u, v;
};

struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
	float x, y, z, w;
};


class Object
{
public:
	Object() = default;
	virtual ~Object() = default;
public:
	virtual HRESULT				CreateVertexBuffer(ID3D11Device* pDevice);
	virtual HRESULT				CreateIndexBuffer(ID3D11Device* pDevice);
	virtual HRESULT				CreateConstantBuffer(ID3D11Device* pDevice);
	virtual HRESULT				CreateTexture(ID3D11Device* pDevice, const std::tstring Name, const std::tstring Filepath);
	virtual HRESULT				CreateShader(ID3D11Device* pDevice, const std::tstring Name, const std::tstring Filepath);
	virtual HRESULT				CreateInputLayout(ID3D11Device* pDevice);
	virtual HRESULT				SetBlendState(ID3D11Device* pDevice);
public:
	virtual bool				Init();
	virtual bool				Frame();
	virtual bool				PreRender(ID3D11DeviceContext* pContext);
	virtual bool				Render(ID3D11DeviceContext* pContext);
	virtual bool				PostRender(ID3D11DeviceContext* pContext);
	virtual bool				Release();
	bool						Create(ID3D11Device* pDevice, const std::tstring& Name ,const std::tstring& ShaderPath, const std::tstring& TexturePath);
protected:
	VS_CB						m_ConstantData;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11InputLayout*			m_pVertexLayout;

	std::vector<PCT_VERTEX>		m_vertexList;
	std::vector<DWORD>			m_indexList;

	std::tstring				m_TextureName;
	Texture*					m_pTexture;
	Shader*						m_pShader;

	ID3D11BlendState*			m_pAlphaBlend;
};