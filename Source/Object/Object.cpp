#include "Object.h"


HRESULT	Object::CreateVertexBuffer()
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

	hr = m_pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
HRESULT Object::CreateIndexBuffer()
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

	hr = m_pDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT Object::CreateConstantBuffer()
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

	hr = m_pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
HRESULT	Object::LoadShaderAndInputLayout()
{
	HRESULT hr;
	ID3DBlob* pVSBuf = nullptr;
	ID3DBlob* pErrMsg = nullptr;
	DWORD dwFlag = D3DCOMPILE_DEBUG;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"VERTEXSHADER", "vs_5_0", dwFlag, NULL, NULL, &pVSBuf, &pErrMsg, NULL)))
	{
		OutputDebugStringA(LPCSTR(pErrMsg->GetBufferPointer()));
	}
	// ���̴� ������ �� ���(������Ʈ ����, ��������)
	V_RETURN(m_pDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize()
		, nullptr, &m_pVertexShader));

	D3D11_INPUT_ELEMENT_DESC layout[] =
	{
		{"POSITION",	0, DXGI_FORMAT_R32G32B32_FLOAT,		0,	0,	D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"COLOR",		0, DXGI_FORMAT_R32G32B32A32_FLOAT,	0,	12,	D3D11_INPUT_PER_VERTEX_DATA ,0},
		{"TEXCOORD",	0, DXGI_FORMAT_R32G32_FLOAT,		0,	28,	D3D11_INPUT_PER_VERTEX_DATA ,0}
	};

	int iNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	m_pDevice->CreateInputLayout(layout, iNum, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);

	ID3DBlob* pPSBuf = nullptr;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"PIXELSHADER", "ps_5_0", dwFlag, NULL, NULL, &pPSBuf, NULL, NULL)))
	{
		OutputDebugStringA(LPCSTR(pErrMsg->GetBufferPointer()));
	}

	V_RETURN(m_pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize()
		, nullptr, &m_pPixelShader));

	pVSBuf->Release();
	pPSBuf->Release();
	return hr;
}
HRESULT Object::LoadTextureFile(ID3D11ShaderResourceView** pTexSRV, const TCHAR* szFileName)
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
HRESULT	Object::SetBlendState()
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

	V_RETURN(m_pDevice->CreateBlendState(&bd, &m_pAlphaBlend1));
	return hr;
}

bool Object::Init()
{
	LoadTextureFile(&m_pTexSRV, L"../../data/effect/Particle6.dds");
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	SetBlendState();
	if (FAILED(LoadShaderAndInputLayout()))
	{
		return false;
	}
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
#ifdef GPU
	//gpu update
	m_pContext->UpdateSubresource(m_pConstantBuffer, 0, nullptr, &m_ConstantData, 0, 0);
#endif
#ifdef CPU
	//cpu update
	// MAP	->  �ڹ��迭��
	D3D11_MAPPED_SUBRESOURCE address;
	if (SUCCEEDED(m_pContext->Map(m_pConstantBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &address)))
	{
		// ������� ����
		VS_CB* cb = (VS_CB*)address.pData;
		*cb = m_ConstantData;
		// UNMAP -> �ݰ�
		m_pContext->Unmap(m_pConstantBuffer, 0);
	}
#endif
	return true;
}
bool Object::Render()
{
	UINT offset = 0;
	UINT stride = sizeof(PCT_VERTEX);

	//���� ����
	//pContext->OMSetBlendState(m_pAlphaBlend1, 0, -1);

	m_pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	m_pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	m_pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_pContext->IASetInputLayout(m_pVertexLayout);
	m_pContext->VSSetShader(m_pVertexShader, NULL, 0);
	//	pContext->PSSetShader(m_pPixelShader, NULL, 0);

		// ���ý�����Ʈ ����(�ȼ� ���̴�)
	m_pContext->PSSetSamplers(0, 1, &m_pSamplerState);
	// �ؽ��Ĺ��� ����(�ȼ� ���̴�)
	// ������� ����(�ȼ� ���̴�)
	m_pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	// ������� ����(���ؽ� ���̴�)
	m_pContext->VSSetConstantBuffers(
		0,	// ���Թ�ȣ(������Ʈ�� ��ȣ)
		1,	// ������� 1��
		&m_pConstantBuffer);

	m_pContext->PSSetShader(m_pPixelShader, NULL, 0);
	m_pContext->PSSetShaderResources(0, 1, &m_pTexSRV);
	m_pContext->OMSetBlendState(m_pAlphaBlend1, 0, -1);
	m_pContext->DrawIndexed(CASTING(UINT32, m_indexList.size()), 0, 0);

	return true;
}
bool Object::Release()
{
	if (m_pVertexBuffer) m_pVertexBuffer->Release();
	if (m_pVertexLayout) m_pVertexLayout->Release();
	if (m_pPixelShader) m_pPixelShader->Release();
	if (m_pVertexShader) m_pVertexShader->Release();
	m_pVertexBuffer = nullptr;
	m_pVertexLayout = nullptr;
	m_pPixelShader = nullptr;
	m_pVertexShader = nullptr;
	return true;
}
bool Object::Create(ID3D11Device* pd3dDevice, std::tstring szShaderName, std::tstring szTexName)
{
	m_iTexindex = S_TextureMgr.Load(pd3dDevice, szTexName);
	m_pTexture = S_TextureMgr.getPtr(m_iTexindex);
}


void Object::Set(ID3D11Device* pDevice, ID3D11DeviceContext* pContext)
{
	m_pDevice = pDevice;
	m_pContext = pContext;
}