#pragma once
#include "Core.h"
#include <D3DX11.h>

#pragma comment(lib,"d3dx11.lib")

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	HRESULT			CreateVertexBuffer	();
	HRESULT			LoadShaderAndInputLayout();
	bool			Init				() override;
	bool			Render				() override;
	bool			Release				() override;
private:
	ID3D11Buffer*		m_pVertexBuffer;
	ID3D11VertexShader* m_pVertexShader;
	ID3D11PixelShader*	m_pPixelShader;
	ID3D11InputLayout*	m_pVertexLayout;
};