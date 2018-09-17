#include "Object.h"


HRESULT	Object::CreateVertexBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = CASTING(UINT, m_vertexList.size()) * sizeof(PCT_VERTEX);

#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#elif defined GPU
	bd.Usage = D3D11_USAGE_DEFAULT;
#endif
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	
	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vertexList.at(0);
	//sd.SysMemPitch;
	//sd.SysMemSlicePitch;

	V_RETURN(pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer));
	return hr;
}
HRESULT Object::CreateIndexBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;

	int iNumCount = CASTING(int, m_indexList.size());

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = iNumCount * sizeof(PCT_VERTEX);
	bd.Usage = D3D11_USAGE_DEFAULT;

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_indexList.at(0);

	V_RETURN(pDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer));
	return hr;
}
HRESULT Object::CreateConstantBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = sizeof(VS_CB);
#ifdef CPU
	bd.Usage = D3D11_USAGE_DYNAMIC;
	bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
#elif defined GPU
	bd.Usage = D3D11_USAGE_DEFAULT;
#endif
	bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

	V_RETURN(pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer));
	return hr;
}
HRESULT Object::CreateTexture(ID3D11Device* pDevice, const std::tstring Name, const std::tstring Filepath)
{
	std::tstring Tex = S_Texture.LoadTexture(pDevice, Name, Filepath);
	m_pTexture = S_Texture.getTexturePtr(Tex);
	return S_OK;
}
HRESULT Object::CreateShader(ID3D11Device* pDevice, const std::tstring Name, const std::tstring Filepath)
{
	std::tstring Shader = S_Shader.LoadShader(pDevice, Name, Filepath);
	m_pShader = S_Shader.getShaderPtr(Shader);
	return S_OK;
}
HRESULT Object::CreateInputLayout(ID3D11Device* pDevice)
{
	HRESULT hr;
	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{ "POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,	D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{ "COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA ,0 },
		{ "TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	28,	D3D11_INPUT_PER_VERTEX_DATA ,0 }
	};

	int iNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	ID3DBlob* pBlob = m_pShader->getBlob();
	V_RETURN(pDevice->CreateInputLayout(layout, iNum, pBlob->GetBufferPointer(), pBlob->GetBufferSize(), &m_pVertexLayout));
	return hr;
}
HRESULT	Object::SetBlendState(ID3D11Device* pDevice)
{
	HRESULT hr;
	D3D11_BLEND_DESC bd;
	ZeroMemory(&bd, sizeof(bd));
	bd.RenderTarget[0].BlendEnable = TRUE;
	// 칼라 블랜드 FINALCOLOR = deskcolor * DestBlend + srccolor * SrcBlend;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// 알파 블랜드 FINALALPHA = deskalpha * DestBlendAlpha + srcalpha * SrcBlendAlpha;
	bd.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	bd.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ZERO;
	bd.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;

	bd.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

	V_RETURN(pDevice->CreateBlendState(&bd, &m_pAlphaBlend));
	return hr;
}

bool Object::Init()
{
	return true;
}
bool Object::Frame()
{
	static float fSpeed = 50.0f;
	static int iNum = 1;
	static float fAngle = 0.0f;
	fSpeed = 50.0f;
	static float fScale = 1.0f;
	m_ConstantData.fTime[3] = cosf(DegreeToRadian(fScale));
	if (S_Input.getKeyState(DIK_INSERT) == KEYSTATE::KEY_PUSH)
	{
		iNum *= -1;
	}
	if (S_Input.getKeyState(DIK_HOME) == KEYSTATE::KEY_HOLD)
	{
		fSpeed = 100.0f;
	}
	if (S_Input.getKeyState(DIK_DELETE) == KEYSTATE::KEY_HOLD)
	{
		fSpeed = 25.0f;
	}
	if (S_Input.getKeyState(DIK_PGUP) == KEYSTATE::KEY_HOLD)
	{
		fScale += g_fSecPerFrame * 50.0f;
	}

	fAngle += (iNum)*g_fSecPerFrame * fSpeed;
	m_ConstantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.b = cosf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.a = 0.3f;
	m_ConstantData.fTime[0] = g_fGameTimer;
	m_ConstantData.fTime[1] = 1.2f;
	m_ConstantData.fTime[2] = DegreeToRadian(fAngle);
	return true;
}
bool Object::PreRender(ID3D11DeviceContext* pContext)
{
#ifdef GPU
	//gpu update
	pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_ConstantData, 0, 0);
#endif
#ifdef CPU
	//cpu update
	// MAP	->  자물쇠열고
	D3D11_MAPPED_SUBRESOURCE address;
	if (SUCCEEDED(pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
	{
		// 상수버퍼 갱신
		VS_CB* cb = (VS_CB*)address.pData;
		*cb = m_ConstantData;
		// UNMAP -> 닫고
		pContext->Unmap(m_pConstantBuffer, 0);
	}
#endif
	UINT offset = 0;
	UINT stride = sizeof(PCT_VERTEX);
	ID3D11VertexShader * pVertexShader = m_pShader->getVertexShader();
	ID3D11PixelShader* pPixelShader = m_pShader->getPixelShader();
	ID3D11SamplerState* pSamplerState = m_pTexture->getSamplerState();
	ID3D11ShaderResourceView* pSRV = m_pTexture->getTexSRV();
	//블랜드 적용
	//pContext->OMSetBlendState(m_pAlphaBlend1, 0, -1);

	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(pVertexShader, NULL, 0);
	//	pContext->PSSetShader(m_pPixelShader, NULL, 0);

	// 샘플스테이트 적용(픽셀 쉐이더)
	pContext->PSSetSamplers(0, 1, &pSamplerState);
	// 텍스쳐버펴 적용(픽셀 쉐이더)
	// 상수버퍼 적용(픽셀 쉐이더)
	pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	// 상수버퍼 적용(버텍스 쉐이더)
	pContext->VSSetConstantBuffers(
		0,	// 슬롯번호(레지스트리 번호)
		1,	// 상수버퍼 1개
		&m_pConstantBuffer);

	pContext->PSSetShader(pPixelShader, NULL, 0);
	pContext->PSSetShaderResources(0, 1, &pSRV);
	pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
	return true;
}
bool Object::Render(ID3D11DeviceContext* pContext)
{
	PreRender(pContext);
	PostRender(pContext);
	return true;
}
bool Object::PostRender(ID3D11DeviceContext* pContext)
{
	pContext->DrawIndexed(CASTING(UINT32, m_indexList.size()), 0, 0);
	return true;
}
bool Object::Release()
{
	RELEASE(m_pVertexBuffer);
	RELEASE(m_pVertexLayout);
	RELEASE(m_pConstantBuffer);
	RELEASE(m_pIndexBuffer);
	return true;
}
bool Object::Create(ID3D11Device* pDevice, const std::tstring& Name, const std::tstring& ShaderPath, const std::tstring& TexturePath)
{
	CreateVertexBuffer(pDevice);
	CreateIndexBuffer(pDevice);
	CreateConstantBuffer(pDevice);
	CreateShader(pDevice, Name, ShaderPath);
	CreateTexture(pDevice, Name, TexturePath);
	CreateInputLayout(pDevice);
	SetBlendState(pDevice);
	return true;
}