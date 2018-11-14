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

enum class E_BSS : unsigned char
{
	Default = 0,
	No,
	Count
};

enum class E_SS : unsigned char
{
	Default = 0,
	Count
};

class DxState
{
public:
	static std::array<Microsoft::WRL::ComPtr<ID3D11BlendState>, (int)E_BSS::Count> m_BSS;
	static std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> m_DSS;
	static std::array<Microsoft::WRL::ComPtr<ID3D11RasterizerState>, (int)E_RSS::Count> m_RSS;
	static std::array<Microsoft::WRL::ComPtr<ID3D11SamplerState>, (int)E_SS::Count> m_SS;
	static HRESULT InitState(ID3D11Device* pd3dDevice);

private:
	static HRESULT InitDepthStencliState(ID3D11Device* pd3dDevice);
	static HRESULT InitRasterizerState(ID3D11Device* pd3dDevice);
	static HRESULT InitBlendState(ID3D11Device* pd3dDevice);
	static HRESULT InitSamplerState(ID3D11Device* pd3dDevice);
};