#pragma once
#include "Core.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include "TextureMgr.h"

#pragma comment(lib,"d3dx11.lib")

#define PI 3.141592
#define DegreeToRadian(x) CASTING(FLOAT,(x) / 180 * PI)

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
	HRESULT						CreateVertexBuffer();
	HRESULT						CreateIndexBuffer();
	HRESULT						CreateConstantBuffer();
	HRESULT						LoadShaderAndInputLayout();
	HRESULT						LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName);
	HRESULT						SetBlendState();
public:
	bool						Init();
	bool						Frame();
	bool						Render();
	bool						Release();
	bool						Create(ID3D11Device* pd3dDevice, std::tstring szShaderName, std::tstring szTexName);
public:
	void						Set(ID3D11Device* pDevice, ID3D11DeviceContext* pContext);
private:
	ID3D11DeviceContext*		m_pContext;
	ID3D11Device*				m_pDevice;
private:
	VS_CB						m_ConstantData;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	Shader*		m_pShaderVS;
	Shader*		m_pShaderPS;
	ID3D11InputLayout*			m_pVertexLayout;
	std::vector<PCT_VERTEX>		m_vertexList;
	std::vector<DWORD>			m_indexList;

	UINT						m_iTexindex;
	Texture*					m_pTexture;

	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11BlendState*			m_pAlphaBlend1;
};