#pragma once
#include "Define.h"

enum class E_DSS : unsigned char
{
	Default = 0,
	Disable,
	Count
};

class DxState
{
public:
	static std::array<Microsoft::WRL::ComPtr<ID3D11DepthStencilState>, (int)E_DSS::Count> m_DSS;
	static HRESULT SetState(ID3D11Device* pd3dDevice);
};