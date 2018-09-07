#include "Sample.h"

struct P3VERTEX
{
	float x, y, z;
};

Sample::Sample() : m_pVertexBuffer(nullptr), m_pVertexShader(nullptr), m_pPixelShader(nullptr), m_pVertexLayout(nullptr)
{}
Sample::~Sample()
{}

bool Sample::Init()
{
	CreateVertexBuffer();
	if (FAILED(LoadShaderAndInputLayout()))
	{
		return false;
	}
	return true;
}
bool Sample::Render()
{
	UINT offset = 0;
	UINT stride = sizeof(P3VERTEX);
	ID3D11DeviceContext * pContext = getContext();
	pContext->IASetVertexBuffers(0,1,&m_pVertexBuffer, &stride, &offset);
	pContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	pContext->IASetInputLayout(m_pVertexLayout);
	pContext->VSSetShader(m_pVertexShader, NULL, 0);
	pContext->PSSetShader(m_pPixelShader, NULL, 0);

	pContext->Draw(6, 0);
	return true;
}
bool Sample::Release()
{
	return true;
}
HRESULT	Sample::CreateVertexBuffer()
{
	HRESULT hr;
	P3VERTEX vList[6]; // 정점의 저장순서 : 시계방향
	vList[0].x = -1.0f,	vList[0].y = 1.0f,	vList[0].z = 0.5f;
	vList[1].x = 1.0f,	vList[1].y = 1.0f,	vList[1].z = 0.5f;
	vList[2].x = -1.0f, vList[2].y = -1.0f,	vList[2].z = 0.5f;

	vList[3].x = -1.0f, vList[3].y = -1.0f, vList[3].z = 0.5f;
	vList[4].x = 1.0f, vList[4].y = 1.0f, vList[4].z = 0.5f;
	vList[5].x = 1.0f, vList[5].y = -1.0f, vList[5].z = 0.5f;

	D3D11_BUFFER_DESC bd;
	ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
	bd.ByteWidth = 6 * sizeof(P3VERTEX);	 // 36바이트
	bd.Usage = D3D11_USAGE_DEFAULT;			 // GPU에 할당

	//Usage
	//D3D11_USAGE_DEFAULT = 0,				 // gpu w/r가능
	//D3D11_USAGE_IMMUTABLE = 1,			 // gpu r 가능
	//D3D11_USAGE_DYNAMIC = 2,				 // 중간에 변경가능
	//D3D11_USAGE_STAGING = 3				 // cpu w/r 가능

	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER; // 정점버퍼

	//bd.CPUAccessFlags = 0;
	//bd.MiscFlags;
	//bd.StructureByteStride;

	D3D11_SUBRESOURCE_DATA sd;
	ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));
	sd.pSysMem = vList;
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
HRESULT	Sample::LoadShaderAndInputLayout()
{
	HRESULT hr;
	ID3D11Device * pDevice = getDevice();
	ID3DBlob* pVSBuf = nullptr;
	V_RETURN(D3DX11CompileFromFile(L"VertexShader.txt", NULL, NULL,
		"VERTEXSHADER", "vs_5_0", NULL, NULL, NULL, &pVSBuf, NULL, NULL));
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
		"PIXELSHADER", "ps_5_0", NULL, NULL, NULL, &pPSBuf, NULL, NULL));
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