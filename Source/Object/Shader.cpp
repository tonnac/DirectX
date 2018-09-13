#include "Shader.h"

bool Shader::Init()
{
	return true;
}
bool Shader::Frame()
{
	return true;
}
bool Shader::Render()
{
	return true;
}
bool Shader::Release()
{
	if (m_pTexSRV) m_pTexSRV->Release();
	if (m_pSamplerState) m_pSamplerState->Release();
	m_pSamplerState = nullptr;
	m_pTexSRV = nullptr;
	return true;
}
HRESULT Shader::CreateShader(ID3D11Device* pDevice, const std::tstring& Filename, const std::string& vs, const std::string& ps)
{
	HRESULT hr;
	ID3DBlob* pVSBuf = nullptr;
	ID3DBlob* pErrMsg = nullptr;
	DWORD dwFlag = D3DCOMPILE_DEBUG;
	if (FAILED(D3DX11CompileFromFile(Filename.c_str(), NULL, NULL,
		vs.c_str(), "vs_5_0", dwFlag, NULL, NULL, &pVSBuf, &pErrMsg, NULL)))
	{
		OutputDebugStringA(LPCSTR(pErrMsg->GetBufferPointer()));
	}
	// 쉐이더 컴파일 된 결과(오브젝트 파일, 목적파일)
	V_RETURN(pDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize()
		, nullptr, &m_pVertexShader));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,	D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	28,	D3D11_INPUT_PER_VERTEX_DATA ,0}
	};

	int iNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	pDevice->CreateInputLayout(layout, iNum, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);

	ID3DBlob* pPSBuf = nullptr;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		ps.c_str(), "ps_5_0", dwFlag, NULL, NULL, &pPSBuf, NULL, NULL)))
	{
		OutputDebugStringA(LPCSTR(pErrMsg->GetBufferPointer()));
	}

	V_RETURN(pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize()
		, nullptr, &m_pPixelShader));

	//

	pPSBuf = nullptr;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"PIXELSHADER1", "ps_5_0", dwFlag, NULL, NULL, &pPSBuf, NULL, NULL)))
	{
		OutputDebugStringA(LPCSTR(pErrMsg->GetBufferPointer()));
	}

	V_RETURN(pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize()
		, nullptr, &m_pPixelShader1));

	//

	pPSBuf = nullptr;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"PIXELSHADER2", "ps_5_0", dwFlag, NULL, NULL, &pPSBuf, NULL, NULL)))
	{
		OutputDebugStringA(LPCSTR(pErrMsg->GetBufferPointer()));
	}

	V_RETURN(pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize()
		, nullptr, &m_pPixelShader2));

	pVSBuf->Release();
	pPSBuf->Release();
	return hr;
	return hr;
}


//HRESULT Shader::LoadShaderFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName)
//{
//	HRESULT hr;
//
//	hr = D3DX11CreateShaderResourceViewFromFile(m_pDevice, szFileName, nullptr, nullptr, pTexSRV, nullptr);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//
//	D3D11_SAMPLER_DESC samplerdesc;
//	ZeroMemory(&samplerdesc, sizeof(D3D11_SAMPLER_DESC));
//	samplerdesc.AddressU = D3D11_Shader_ADDRESS_WRAP;
//	samplerdesc.AddressV = D3D11_Shader_ADDRESS_WRAP;
//	samplerdesc.AddressW = D3D11_Shader_ADDRESS_WRAP;
//	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
//
//	hr = m_pDevice->CreateSamplerState(&samplerdesc, &m_pSamplerState);
//	if (FAILED(hr))
//	{
//		return hr;
//	}
//
//	return hr;
//}