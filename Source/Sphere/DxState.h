#pragma once
#include "Define.h"

class DxState
{
public:
	enum class E_DSS : unsigned char
	{
		DEFAULT,
		DISABLE
	};

	static std::unordered_map<E_DSS, Microsoft::WRL::ComPtr<ID3D11DepthStencilState>> m_DSS;
	static HRESULT SetState(ID3D11Device* pd3dDevice);
};