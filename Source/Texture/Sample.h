#pragma once
#include "Core.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>

#pragma comment(lib,"d3dx11.lib")

#define PI 3.141592
#define DegreeToRadian(x) CASTING(FLOAT,(x) / 180 * PI)

#define GPU
//#define CPU

struct P3VERTEX // pnct
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

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	HRESULT						CreateVertexBuffer		();
	HRESULT						CreateIndexBuffer		();
	HRESULT						CreateConstantBuffer	();
	HRESULT						LoadShaderAndInputLayout();
	HRESULT						LoadTextureFile			(const TCHAR* szFileName);
	bool						Init					() override;
	bool						Frame					() override;
	bool						Render					() override;
	bool						Release					() override;
private:
	VS_CB						m_ConstantData;
	ID3D11Buffer*				m_pVertexBuffer;
	ID3D11Buffer*				m_pIndexBuffer;
	ID3D11Buffer*				m_pConstantBuffer;
	ID3D11VertexShader*			m_pVertexShader;
	ID3D11PixelShader*			m_pPixelShader;
	ID3D11InputLayout*			m_pVertexLayout;
	std::vector<P3VERTEX>		m_vertexList;
	std::vector<DWORD>			m_indexList;
	ID3D11ShaderResourceView*	m_pTexSRV;
	ID3D11SamplerState*			m_pSamplerState;
};