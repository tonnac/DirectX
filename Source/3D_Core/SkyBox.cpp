
#include "SkyBox.h"
#include "DxState.h"

using Microsoft::WRL::ComPtr;

void SkyBox::Create(ID3D11Device* pDevice, std::tstring szShaderName, std::tstring szTextureName)
{
	Shape::Create(pDevice, szShaderName, szTextureName);
}

HRESULT SkyBox::LoadVertexShader(std::tstring szName)
{
	HRESULT hr = S_OK;

	switch (m_bRendertype)
	{
	case 2:
		m_DxObject.m_pVertexShader.Attach(DX::LoadVertexShaderFile(m_pDevice, szName.c_str(), &m_DxObject.m_pVSBlob, "VSCube"));
		break;
	default:
		BoxShape::LoadVertexShader(szName);
		break;
	}
	return hr;
}

HRESULT SkyBox::LoadPixelShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	switch (m_bRendertype)
	{
	case 0:
		BoxShape::LoadPixelShader(szName);
		break;
	case 1:
		m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(m_pDevice, szName.c_str(), nullptr, "PSArray");
		break;
	case 2:
		m_DxObject.m_pPixelShader = DX::LoadPixelShaderFile(m_pDevice, szName.c_str(), nullptr, "PSCube");
		break;
	default:
		break;
	}
	return hr;
}

HRESULT SkyBox::LoadTextureShader(std::tstring szName)
{
	HRESULT hr = S_OK;
	if (m_bRendertype == 2)
	{
		BoxShape::LoadTextureShader(szName);
	}
	else
	{
		const TCHAR* szSkyTexture[] =
		{
			L"../../data/sky/st00_cm_front.bmp",
			L"../../data/sky/st00_cm_back.bmp",
			L"../../data/sky/st00_cm_right.bmp",
			L"../../data/sky/st00_cm_left.bmp",
			L"../../data/sky/st00_cm_up.bmp",
			L"../../data/sky/st00_cm_down.bmp"
		};

		for (int i = 0; i < MAX_SKYBOX_TEXTURE; ++i)
		{
			m_pTexSRV[i] = DX::CreateShaderResourceView(m_pDevice, szSkyTexture[i]);
		}
	}
	return hr;
}

bool SkyBox::Render(ID3D11DeviceContext* pContext)
{
	D3DXMATRIX matView = m_matView;
	m_matView._41 = 0;
	m_matView._42 = 0;
	m_matView._43 = 0;

	SetMatrix(&m_matWorld, &m_matView, &m_matProj);
	PreRender(pContext);
	if (m_bRendertype == 0)
	{
		for (int i = 0; i < MAX_SKYBOX_TEXTURE; ++i)
		{
			pContext->PSSetShaderResources(0, 1, m_pTexSRV[i].GetAddressOf());
			pContext->DrawIndexed(6, 6 * i, 0);
		}
	}
	else if(m_bRendertype == 1)
	{
		pContext->PSSetShaderResources(1, (UINT)std::size(m_pTexSRV), m_pTexSRV[0].GetAddressOf());
		pContext->DrawIndexed(m_DxObject.m_iNumIndex, 0, 0);
	}
	else
	{
		pContext->PSSetShaderResources(2, 1, m_DxObject.m_pTextureSRV.GetAddressOf());
		pContext->DrawIndexed(m_DxObject.m_iNumIndex, 0, 0);
	}
	return true;
}