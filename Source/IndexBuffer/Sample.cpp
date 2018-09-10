#include "Sample.h"



Sample::Sample() : m_pVertexBuffer(nullptr), m_pVertexShader(nullptr), m_pPixelShader(nullptr), m_pVertexLayout(nullptr)
{}
Sample::~Sample()
{}

bool Sample::Init()
{
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
	static int iNum = 0;
	static float fAngle = 0.0f;
	ID3D11DeviceContext* pContext = getContext();
	m_ConstantData.r = cosf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.g = sinf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.b = cosf(g_fGameTimer) * 0.5f + 0.5f;
	m_ConstantData.a = 1.3f;
	m_ConstantData.fTime[0] = g_fGameTimer;
	m_ConstantData.fTime[1] = 1.2f;
	m_ConstantData.fTime[2] = DegreeToRadian(fAngle);

	switch (iNum)
	{
	case 0:
		m_ConstantData.x = -0.5f, m_ConstantData.y = 0.5f, m_ConstantData.z = m_vertexList[0].z;
		break;
	case 1:
		m_ConstantData.x = 0.5f, m_ConstantData.y = 0.5f, m_ConstantData.z = m_vertexList[1].z;
		break;
	case 2:
		m_ConstantData.x = -0.5f, m_ConstantData.y = -0.5f, m_ConstantData.z = m_vertexList[2].z;
		break;
	case 3:
		m_ConstantData.x = 0.5f, m_ConstantData.y = -0.5f, m_ConstantData.z = m_vertexList[3].z;
		break;
	}
	P3VERTEX ple;
	ple.x = m_ConstantData.x * cos(fAngle) - m_ConstantData.x * sin(fAngle);
	ple.y = m_ConstantData.y * cos(fAngle) + m_ConstantData.y * sin(fAngle);
	ple.z = m_ConstantData.z;
	if (++iNum >= 4)
	{
		fAngle += g_fSecPerFrame * 10.0f;
		iNum = 0;
	}
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
	m_vertexList[0].x = -0.5f,	m_vertexList[0].y = 0.5f,	m_vertexList[0].z = 0.5f;
	m_vertexList[1].x = 0.5f,	m_vertexList[1].y = 0.5f,	m_vertexList[1].z = 0.5f;
	m_vertexList[2].x = -0.5f,	m_vertexList[2].y = -0.5f,	m_vertexList[2].z = 0.5f;
	m_vertexList[3].x = 0.5f,	m_vertexList[3].y = -0.5f,	m_vertexList[3].z = 0.5f;

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
		std::string pe((char*)pErrMsg->GetBufferPointer());
		OutputDebugString((LPCWSTR)(pErrMsg->GetBufferPointer()));
	}
	// 쉐이더 컴파일 된 결과(오브젝트 파일, 목적파일)
	V_RETURN(pDevice->CreateVertexShader(pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize()
		, nullptr, &m_pVertexShader));

	D3D11_INPUT_ELEMENT_DESC layout;
	layout.SemanticName = "POSITION";
	layout.SemanticIndex = 0;
	layout.Format = DXGI_FORMAT_R32G32B32_FLOAT;
	layout.InputSlot = 0;
	layout.AlignedByteOffset = 0;
	layout.InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	layout.InstanceDataStepRate = 0;

	int iNum = sizeof(layout) / sizeof(D3D11_INPUT_ELEMENT_DESC);

	pDevice->CreateInputLayout(&layout, iNum, pVSBuf->GetBufferPointer(), pVSBuf->GetBufferSize(), &m_pVertexLayout);

	ID3DBlob* pPSBuf = nullptr;
	V_RETURN(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"PIXELSHADER", "ps_5_0", dwFlag, NULL, NULL, &pPSBuf, NULL, NULL));
	V_RETURN(pDevice->CreatePixelShader(pPSBuf->GetBufferPointer(), pPSBuf->GetBufferSize()
		, nullptr, &m_pPixelShader));

	pVSBuf->Release();
	pPSBuf->Release();
	return hr;
}
int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE prevInst, LPWSTR szCmdLine, int nCmdShow)
{
	Sample wd;
	wd.InitWindow(hInstance, 800, 600, nCmdShow, L"SampleVertexBuffer");
	wd.Run();
	return 0;
}