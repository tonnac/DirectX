#pragma once
#include "Core.h"
#include <D3DX11.h>
#include <D3Dcompiler.h>
#include <D3DX10math.h>

#pragma comment(lib,"d3dx11.lib")
#pragma comment(lib,"d3dx10.lib")

#define PI 3.141592
#define DegreeToRadian(x) CASTING(FLOAT,(x) / 180 * PI)

#define GPU
//#define CPU

struct P3VERTEX
{
	float x, y, z;
};

struct VS_CB
{
	float r, g, b, a;
	float fTime[4];
	float x, y, z, w;
	D3DXMATRIX toProj;
};

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	HRESULT					CreateVertexBuffer		();
	HRESULT					CreateIndexBuffer		();
	HRESULT					CreateConstantBuffer();
	HRESULT					LoadShaderAndInputLayout();
	bool					Init					() override;
	bool					Frame					() override;
	bool					Render					() override;
	bool					Release					() override;
private:
	VS_CB					m_ConstantData;
	ID3D11Buffer*			m_pVertexBuffer;
	ID3D11Buffer*			m_pIndexBuffer;
	ID3D11Buffer*			m_pConstantBuffer;
	ID3D11VertexShader*		m_pVertexShader;
	ID3D11PixelShader*		m_pPixelShader;
	ID3D11InputLayout*		m_pVertexLayout;
	std::vector<P3VERTEX>	m_vertexList;
	std::vector<DWORD>		m_indexList;
};