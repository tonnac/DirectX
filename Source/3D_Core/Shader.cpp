#include "Shader.h"

bool Shader::Release()
{
	RELEASE(m_pVertexShader);
	RELEASE(m_pPixelShader);
	RELEASE(m_pVSBlob);
	return true;
}
HRESULT Shader::CreateShader(ID3D11Device* pDevice, const std::tstring& Filename, const std::string& vs, const std::string& ps)
{
	HRESULT hr;
	ID3DBlob* pErrBlob = nullptr;
	ifShaderFailed(D3DX11CompileFromFile(Filename.c_str(), NULL, NULL, vs.c_str(), "vs_5_0", m_dwFlag, NULL, NULL, &m_pVSBlob, &pErrBlob, NULL));
	// 쉐이더 컴파일 된 결과(오브젝트 파일, 목적파일)
	V_RETURN(pDevice->CreateVertexShader(m_pVSBlob->GetBufferPointer(), m_pVSBlob->GetBufferSize(), nullptr, &m_pVertexShader));
	ID3DBlob* pPSBuf = nullptr;
	ifShaderFailed(D3DX11CompileFromFile(Filename.c_str(), NULL, NULL, ps.c_str(), "ps_5_0", m_dwFlag, NULL, NULL, &pPSBuf, &pErrBlob, NULL));
	V_RETURN(pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize(), nullptr, &m_pPixelShader));

	pPSBuf->Release();
	return hr;
}
ID3D11VertexShader * Shader::getVertexShader()
{
	GETPTR(m_pVertexShader);
}
ID3D11PixelShader* Shader::getPixelShader()
{
	GETPTR(m_pPixelShader);
}
ID3DBlob * Shader::getBlob()
{
	GETPTR(m_pVSBlob);
}
const std::tstring& Shader::getName() const
{
	return m_Name;
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

bool ShaderMgr::Release()
{
	for (auto it : m_ShaderList)
	{
		it.second->Release();
		delete it.second;
	}
	m_ShaderList.clear();
	return true;
}
const std::tstring& ShaderMgr::LoadShader(ID3D11Device * pDevice, const std::tstring& Name, const std::tstring& Filepath, const std::string& vs, const std::string& ps)
{
	if (getShaderPtr(Name))
	{
		return Name;
	}

	SharedIter it;
	it = m_ShaderList.find(Name);
	if (it != m_ShaderList.end())
	{
		AddCache(it->first, it->second);
		return Name;
	}

	Shader * newData = new Shader;
	if (FAILED(newData->CreateShader(pDevice, Filepath, vs, ps))) return nullptr;

	m_ShaderList.insert(std::make_pair(Name, newData));
	AddCache(Name, newData);
	return Name;
}

void ShaderMgr::AddCache(const std::tstring& szName, Shader* pShader)
{
	CacheData_<Shader*> Cache_;
	Cache_.m_Name = szName;
	Cache_.m_Data = pShader;
	if (m_CacheList.size() >= 3)
	{
		m_CacheList.pop_front();
	}
	m_CacheList.push_back(Cache_);
}
Shader* ShaderMgr::getShaderPtr(const std::tstring& Name)
{
	CacheIter it;
	auto foo = [&Name](CacheData_<Shader*> pSha) {if (Name == pSha.m_Name) { return true; } return false;};
	it = std::find_if(m_CacheList.begin(), m_CacheList.end(), foo);
	if (it != m_CacheList.end())
	{
		return it->m_Data;
	}
	if (m_ShaderList.find(Name) != m_ShaderList.end())
	{
		return m_ShaderList[Name];
	}
	return nullptr;
}