#pragma once
#include "Define.h"

enum class E_DSS : unsigned char
{
	Default = 0,
	Disable,
	Greater,
	Count
};

enum class E_RSS : unsigned char
{
	Default = 0,
	Solidfront,
	SolidBack,
	Wireframe,
	Count
};

class DxState
{
public:
	static std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> m_DSS;
	static std::array<Microsoft::WRL::ComPtr<ID3D11RasterizerState>, (int)E_RSS::Count> m_RSS;
	static HRESULT InitState(ID3D11Device* pd3dDevice);
	static HRESULT InitDepthStencliState(ID3D11Device* pd3dDevice);
	static HRESULT InitRasterizerState(ID3D11Device* pd3dDevice);
};