#include "Object.h"


HRESULT	Object::CreateVertexBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;
	const int iNumCount = 4;
	m_vertexList.resize(iNumCount);
	m_vertexList[0].x = -0.5f, m_vertexList[0].y = 0.5f, m_vertexList[0].z = 0.5f;
	m_vertexList[0].u = 0.0f, m_vertexList[0].v = 0.0f;
	m_vertexList[0].r = 1.0f, m_vertexList[0].g = 0.0f, m_vertexList[0].b = 0.0f, m_vertexList[0].a = 0.0f;

	m_vertexList[1].x = 0.5f, m_vertexList[1].y = 0.5f, m_vertexList[1].z = 0.5f;
	m_vertexList[1].u = 1.0f, m_vertexList[1].v = 0.0f;
	m_vertexList[1].r = 0.0f, m_vertexList[1].g = 0.0f, m_vertexList[1].b = 1.0f, m_vertexList[1].a = 0.0f;

	m_vertexList[2].x = -0.5f, m_vertexList[2].y = -0.5f, m_vertexList[2].z = 0.5f;
	m_vertexList[2].u = 0.0f, m_vertexList[2].v = 1.0f;
	m_vertexList[2].r = 0.0f, m_vertexList[2].g = 1.0f, m_vertexList[2].b = 0.0f, m_vertexList[2].a = 0.0f;

	m_vertexList[3].x = 0.5f, m_vertexList[3].y = -0.5f, m_vertexList[3].z = 0.5f;
	m_vertexList[3].u = 1.0f, m_vertexList[3].v = 1.0f;
	m_vertexList[3].r = 1.0f, m_vertexList[3].g = 1.0f, m_vertexList[3].b = 1.0f, m_vertexList[3].a = 1.0f;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = iNumCount * sizeof(PCT_VERTEX);	 // 36����Ʈ
	bd.Usage = D3D11_USAGE_DEFAULT;					 // GPU�� �Ҵ�

	//Usage
	//D3D11_USAGE_DEFAULT = 0,						 // gpu w/r����
	//D3D11_USAGE_IMMUTABLE = 1,					 // gpu r ����
	//D3D11_USAGE_DYNAMIC = 2,						 // �߰��� ���氡��
	//D3D11_USAGE_STAGING = 3						 // cpu w/r ����

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		 // ��������

	//bd.CPUAccessFlags = 0;
	//bd.MiscFlags;
	//bd.StructureByteStride;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vertexList.at(0);
	//sd.SysMemPitch;
	//sd.SysMemSlicePitch;

	hr = pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
HRESULT Object::CreateIndexBuffer(ID3D11Device* pDevice)
{
	HRESULT hr;

	m_indexList.push_back(0);
	m_indexList.push_back(1);
	m_indexList.push_back(2);
	m_indexList.push_back(3);
	m_indexList.push_back(2);
	m_indexList.push_back(1);

	//DWORD indicies[] =
	//{
	//	0,1,2,
	//	3,2,1
	//};

	int iNumCount = CASTING(int, m_indexList.size());

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = iNumCount * sizeof(PCT_VERTEX);
	bd.Usage = D3D11_USAGE_DEFAULT;

	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

	//bd.CPUAccessFlags = 0;
	//bd.MiscFlags;
	//bd.StructureByteStride;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_indexList.at(0);
	//sd.SysMemPitch;
	//sd.SysMemSlicePitch;

	hr = pDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
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

	hr = pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
HRESULT Object::CreateTexture(ID3D11Device* pDevice, const std::tstring Name, const std::tstring Filepath)
{
	std::tstring Tex = S_TextureMgr.LoadTexture(pDevice, Name, Filepath);
	m_pTexture = S_TextureMgr.getTexturePtr(Tex);
	return S_OK;
}
HRESULT Object::CreateShader(ID3D11Device* pDevice, const std::tstring Name, const std::tstring Filepath)
{
	std::tstring Shader = S_ShaderMgr.LoadShader(pDevice, Name, Filepath);
	m_pShader = S_ShaderMgr.getShaderPtr(Shader);
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
	// Į�� ���� FINALCOLOR = deskcolor * DestBlend + srccolor * SrcBlend;
	bd.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA;
	bd.RenderTarget[0].DestBlend = D3D11_BLEND_INV_SRC_ALPHA;
	bd.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	// ���� ���� FINALALPHA = deskalpha * DestBlendAlpha + srcalpha * SrcBlendAlpha;
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
bool Object::Frame(ID3D11DeviceContext* pContext)
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
#ifdef GPU
	//gpu update
	pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_ConstantData, 0, 0);
#endif
#ifdef CPU
	//cpu update
	// MAP	->  �ڹ��迭��
	D3D11_MAPPED_SUBRESOURCE address;
	if (SUCCEEDED(pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
	{
		// ������� ����
		VS_CB* cb = (VS_CB*)address.pData;
		*cb = m_ConstantData;
		// UNMAP -> �ݰ�
		pContext->Unmap(m_pConstantBuffer, 0);
	}
#endif
	return true;
}
bool Object::Render(ID3D11DeviceContext* pContext)
{
	UINT offset = 0;
	UINT stride = sizeof(PCT_VERTEX);
	ID3D11VertexShader * pVertexShader = m_pShader->getVertexShader();
	ID3D11PixelShader* pPixelShader = m_pShader->getPixelShader();
	ID3D11SamplerState* pSamplerState = m_pTexture->getSamplerState();
	ID3D11ShaderResourceView* pSRV = m_pTexture->getTexSRV();
	//���� ����
	//pContext->OMSetBlendState(m_pAlphaBlend1, 0, -1);

	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(pVertexShader, NULL, 0);
	//	pContext->PSSetShader(m_pPixelShader, NULL, 0);

		// ���ý�����Ʈ ����(�ȼ� ���̴�)
	pContext->PSSetSamplers(0, 1, &pSamplerState);
	// �ؽ��Ĺ��� ����(�ȼ� ���̴�)
	// ������� ����(�ȼ� ���̴�)
	pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	// ������� ����(���ؽ� ���̴�)
	pContext->VSSetConstantBuffers(
		0,	// ���Թ�ȣ(������Ʈ�� ��ȣ)
		1,	// ������� 1��
		&m_pConstantBuffer);

	pContext->PSSetShader(pPixelShader, NULL, 0);
	pContext->PSSetShaderResources(0, 1, &pSRV);
	pContext->OMSetBlendState(m_pAlphaBlend, 0, -1);
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