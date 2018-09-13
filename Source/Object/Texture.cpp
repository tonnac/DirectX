#include "Texture.h"

bool Texture::Init()
{
	return true;
}
bool Texture::Frame()
{
	return true;
}
bool Texture::Render()
{
	return true;
}
bool Texture::Release()
{
	if (m_pTexSRV) m_pTexSRV->Release();
	if (m_pSamplerState) m_pSamplerState->Release();
	m_pSamplerState = nullptr;
	m_pTexSRV = nullptr;
	return true;
}
HRESULT Texture::LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName)
{
	HRESULT hr;

	hr = D3DX11CreateShaderResourceViewFromFile(m_pDevice, szFileName, nullptr, nullptr, pTexSRV, nullptr);
	if (FAILED(hr))
	{
		return hr;
	}

	D3D11_SAMPLER_DESC samplerdesc;
	ZeroMemory(&samplerdesc, sizeof(D3D11_SAMPLER_DESC));
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = m_pDevice->CreateSamplerState(&samplerdesc, &m_pSamplerState);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}