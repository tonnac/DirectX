#pragma once

#include "Shape.h"

#define MAX_SKYBOX_TEXTURE 6

class SkyBox : public BoxShape
{
public:
	void Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName = std::tstring());
	HRESULT LoadTextureShader(std::tstring szName);

	HRESULT LoadVertexShader(std::tstring szName);
	HRESULT LoadPixelShader(std::tstring szName);


	bool Render(ID3D11DeviceContext* pContext);
	bool Release()
	{
		BoxShape::Release();
	}
public:
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTexSRV[MAX_SKYBOX_TEXTURE];
	BOOL m_bRendertype = 1;
};
