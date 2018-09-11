#include "Sample.h"
#include "DirectInput.h"


Sample::Sample() : m_pVertexBuffer(nullptr), m_pVertexShader(nullptr), m_pPixelShader(nullptr), m_pVertexLayout(nullptr)
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	LoadTextureFile(L"../../data/bk.bmp");
	CreateVertexBuffer();
	CreateIndexBuffer();
	CreateConstantBuffer();
	if (FAILED(LoadShaderAndInputLayout()))
	{
		return false;
	}
	return true;
}
bool Sample::Frame()
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
	ID3D11DeviceContext* pContext = getContext();
	m_ConstantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.b = cosf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.a = 1.3f;
	m_ConstantData.fTime[0] = g_fGameTimer;
	m_ConstantData.fTime[1] = 0.2f;
	m_ConstantData.fTime[2] = DegreeToRadian(fAngle);
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
	return true;
}
bool Sample::Render()
{
	UINT offset = 0;
	UINT stride = sizeof(P3VERTEX);
	ID3D11DeviceContext * pContext = getContext();
	pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, offset);
	pContext->IASetVertexBuffers(0,1,&m_pVertexBuffer, &stride, &offset);
	
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(m_pVertexShader, NULL, 0);
	pContext->PSSetShader(m_pPixelShader, NULL, 0);

	// 샘플스테이트 적용(픽셀 쉐이더)
	pContext->PSSetSamplers(0, 1, &m_pSamplerState);
	// 텍스쳐버펴 적용(픽셀 쉐이더)
	pContext->PSSetShaderResources(0, 1, &m_pTexSRV);
	// 상수버퍼 적용(픽셀 쉐이더)
	pContext->PSSetConstantBuffers(0, 1, &m_pConstantBuffer);
	// 상수버퍼 적용(버텍스 쉐이더)
	pContext->VSSetConstantBuffers(
	0,	// 슬롯번호(레지스트리 번호)
	1,	// 상수버퍼 1개
	&m_pConstantBuffer);
	
	pContext->DrawIndexed(CASTING(UINT32,m_indexList.size()), 0, 0);
	return true;
}
bool Sample::Release()
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
HRESULT	Sample::CreateVertexBuffer()
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
	bd.ByteWidth = iNumCount * sizeof(P3VERTEX);	 // 36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;					 // GPU에 할당

	//Usage
	//D3D11_USAGE_DEFAULT = 0,						 // gpu w/r가능
	//D3D11_USAGE_IMMUTABLE = 1,					 // gpu r 가능
	//D3D11_USAGE_DYNAMIC = 2,						 // 중간에 변경가능
	//D3D11_USAGE_STAGING = 3						 // cpu w/r 가능

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;		 // 정점버퍼

	//bd.CPUAccessFlags = 0;
	//bd.MiscFlags;
	//bd.StructureByteStride;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = &m_vertexList.at(0);
	//sd.SysMemPitch;
	//sd.SysMemSlicePitch;

	ID3D11Device* pDevice = getDevice();
	hr = pDevice->CreateBuffer(&bd, &sd, &m_pVertexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
HRESULT Sample::CreateIndexBuffer()

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
	bd.ByteWidth = iNumCount * sizeof(P3VERTEX);	 
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

	ID3D11Device* pDevice = getDevice();
	hr = pDevice->CreateBuffer(&bd, &sd, &m_pIndexBuffer);
	if (FAILED(hr))
	{
		return hr;
	}
	return hr;
}
HRESULT Sample::CreateConstantBuffer()
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

	ID3D11Device* pDevice = getDevice();
	hr = pDevice->CreateBuffer(&bd, NULL, &m_pConstantBuffer);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
HRESULT	Sample::LoadShaderAndInputLayout()
{
	HRESULT hr;
	ID3D11Device * pDevice = getDevice();
	ID3DBlob* pVSBuf = nullptr;
	ID3DBlob* pErrMsg = nullptr;
	DWORD dwFlag = D3DCOMPILE_DEBUG;
	if (FAILED(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"VERTEXSHADER", "vs_5_0", dwFlag, NULL, NULL, &pVSBuf, &pErrMsg, NULL)))
	{
		std::string Error((char*)pErrMsg->GetBufferPointer());
		std::fstream fp("Error.txt",std::ios::out);
		fp << Error;
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
		"PIXELSHADER", "ps_5_0", dwFlag, NULL, NULL, &pPSBuf, NULL, NULL)))
	{
		std::string Error((char*)pErrMsg->GetBufferPointer());
		std::fstream fp("Error.txt", std::ios::out);
		fp << Error;
	}

	V_RETURN(pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize()
		, nullptr, &m_pPixelShader));

	pVSBuf->Release();
	pPSBuf->Release();
	return hr;
}
HRESULT Sample::LoadTextureFile(const TCHAR* szFileName)
{
	HRESULT hr;
	ID3D11Device* pDevice = getDevice();

	//D3DX11_IMAGE_LOAD_INFO	imageinfo;
	//D3DX11_IMAGE_INFO		srcinfo;
	//ZeroMemory(&imageinfo, sizeof(D3DX11_IMAGE_LOAD_INFO));
	//imageinfo.Format = DXGI_FORMAT_FROM_FILE;
	//imageinfo.pSrcInfo = &srcinfo;
	//imageinfo.BindFlags = D3D11_BIND_SHADER_RESOURCE;
	//imageinfo.Width = D3DX11_DEFAULT;
	//imageinfo.Height = D3DX11_DEFAULT;
	//imageinfo.MipLevels = 1;

	hr = D3DX11CreateShaderResourceViewFromFile(pDevice, szFileName, nullptr, nullptr , &m_pTexSRV, nullptr);
	if (FAILED(hr))
	{
		return hr;
	}
	//D3D11_SHADER_RESOURCE_VIEW_DESC desc;
	//m_pTexSRV->GetDesc(&desc);

	// samplerstate create

	D3D11_SAMPLER_DESC samplerdesc;
	ZeroMemory(&samplerdesc, sizeof(D3D11_SAMPLER_DESC));
	samplerdesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerdesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	hr = pDevice->CreateSamplerState(&samplerdesc, &m_pSamplerState);
	if (FAILED(hr))
	{
		return hr;
	}

	return hr;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleVertexBuffer");
	wd.Run();
	return 0;
}
