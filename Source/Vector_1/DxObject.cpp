#include "DxObject.h"

namespace DX
{

	ID3D11Buffer* CreateVertexBuffer(ID3D11Device* pDevice, UINT iNumVertex, UINT iVertexSize, LPVOID pData,ID3D11Buffer** ppBuffer)
	{
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.ByteWidth = iVertexSize * iNumVertex;
		bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;

		sd.pSysMem = pData;

		if (FAILED(pDevice->CreateBuffer(&bd, &sd, &pBuffer)))
		{
			return nullptr;
		}
		if (ppBuffer != nullptr)
		{
			*ppBuffer = pBuffer;
		}
		return pBuffer;
	}

	ID3D11Buffer* CreateIndexBuffer(ID3D11Device* pDevice, 
		UINT iNumCount,
		UINT iIndexSize, 
		LPVOID pData,
		ID3D11Buffer** ppBuffer)
	{
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

		bd.Usage = D3D11_USAGE_DEFAULT;
		bd.CPUAccessFlags = 0;
		bd.ByteWidth = iNumCount * iIndexSize;
		bd.BindFlags = D3D11_BIND_INDEX_BUFFER;

		sd.pSysMem = pData;

		if (FAILED(pDevice->CreateBuffer(&bd, &sd, &pBuffer)))
		{
			return nullptr;
		}
		if (ppBuffer != nullptr)
		{
			*ppBuffer = pBuffer;
		}
		return pBuffer;
	}

	ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device* pDevice, 
		UINT iNumCount, 
		UINT iIndexSize, 
		ID3D11Buffer** ppBuffer,
		LPVOID pData,
		bool bDynamic)
	{
		ID3D11Buffer* pBuffer = nullptr;
		D3D11_BUFFER_DESC bd;
		D3D11_SUBRESOURCE_DATA sd;
		ZeroMemory(&bd, sizeof(D3D11_BUFFER_DESC));
		ZeroMemory(&sd, sizeof(D3D11_SUBRESOURCE_DATA));

		if (bDynamic)
		{
			bd.Usage = D3D11_USAGE_DEFAULT;
			bd.CPUAccessFlags = 0;
		}
		else
		{
			bd.Usage = D3D11_USAGE_DYNAMIC;
			bd.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
		}
		bd.ByteWidth = iNumCount * iIndexSize;
		bd.BindFlags = D3D11_BIND_CONSTANT_BUFFER;

		if (pData != nullptr)
		{
			sd.pSysMem = pData;
			if (FAILED(pDevice->CreateBuffer(&bd, &sd, &pBuffer)))
			{
				return nullptr;
			}
		}
		else
		{
			if (FAILED(pDevice->CreateBuffer(&bd, nullptr, &pBuffer)))
			{
				return nullptr;
			}
		}

		if (ppBuffer != nullptr)
		{
			*ppBuffer = pBuffer;
		}
		return pBuffer;
	}

	ID3D11InputLayout* CreateInputLayout(
		ID3D11Device* pDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements,
		ID3D11InputLayout ** ppInputLayout)
	{
		ID3D11InputLayout * pInputLayout = nullptr;
		if (FAILED(pDevice->CreateInputLayout(layout, numElements, lpData, dwSize, &pInputLayout)))
		{
			return nullptr;
		}

		if (ppInputLayout != nullptr)
		{
			*ppInputLayout = pInputLayout;
		}
		return pInputLayout;
	}

	ID3D11VertexShader* LoadVertexShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut,
		const char * pFuncName)
	{
		ID3D11VertexShader * pVertexShader = nullptr;
		ID3DBlob* pBlob = nullptr;
		
		if (FAILED(CompileShaderFromFile((TCHAR*)pShaderFile, pFuncName, "vs_5_0", &pBlob)))
		{
			return nullptr;
		}
		if (FAILED(pDevice->CreateVertexShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pVertexShader)))
		{
			RELEASE(pBlob);
			return nullptr;
		}
		if (ppBlobOut == nullptr)
		{
			RELEASE(pBlob);
		}
		else
		{
			*ppBlobOut = pBlob;
		}
		return pVertexShader;
	}

	ID3D11PixelShader* LoadPixelShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut ,
		const char * pFuncName)
	{
		ID3D11PixelShader * pPixelShader = nullptr;
		ID3DBlob* pBlob = nullptr;

		if (FAILED(CompileShaderFromFile((TCHAR*)pShaderFile, pFuncName, "ps_5_0", &pBlob)))
		{
			return nullptr;
		}
		if (FAILED(pDevice->CreatePixelShader(pBlob->GetBufferPointer(), pBlob->GetBufferSize(), nullptr, &pPixelShader)))
		{
			RELEASE(pBlob);
			return nullptr;
		}
		if (ppBlobOut == nullptr)
		{
			RELEASE(pBlob);
		}
		else
		{
			*ppBlobOut = pBlob;
		}
		return pPixelShader;
	}

	HRESULT CompileShaderFromFile(
		const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut)
	{
		HRESULT hr = S_OK;

		DWORD dwShaderFlags = D3DCOMPILE_ENABLE_STRICTNESS;
#if defined( DEBUG ) || defined( _DEBUG )	
		dwShaderFlags |= D3DCOMPILE_DEBUG;
#endif
		ID3DBlob* pErrorBlob;
		hr = D3DX11CompileFromFile(szFileName, NULL, NULL, szEntryPoint, szShaderModel,
			dwShaderFlags, 0, NULL, ppBlobOut, &pErrorBlob, NULL);
		if (FAILED(hr))
		{
			if (pErrorBlob != NULL)
				OutputDebugStringA((char*)pErrorBlob->GetBufferPointer());
			if (pErrorBlob) pErrorBlob->Release();
			return hr;
		}
		if (pErrorBlob) pErrorBlob->Release();

		return S_OK;
	}

	DxObject::DxObject()
	{
	}
	DxObject::~DxObject()
	{
	}

	bool DxObject::PreRender(ID3D11DeviceContext* pContext,
								UINT iVertexSize)
	{
		pContext->IASetInputLayout(m_pInputLayout);

		UINT stride = iVertexSize;
		UINT offset = 0;
		pContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

		pContext->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		pContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);
		pContext->VSSetShader(m_pVertexShader, nullptr, 0);
		pContext->PSSetShader(m_pPixelShader, nullptr, 0);
		return true;
	}
	bool DxObject::PostRender(ID3D11DeviceContext* pContext,
							UINT iVertexSize, UINT iCount)
	{
		if(m_pIndexBuffer != nullptr)
			pContext->DrawIndexed(iCount, 0, 0);
		else
			pContext->Draw(iCount, 0);
		return true;
	}
	bool DxObject::Render(ID3D11DeviceContext* pContext,
						UINT iVertexSize, UINT iCount)
	{
		PreRender(pContext, iVertexSize);
		PostRender(pContext, iVertexSize, iCount);
		return true;
	}

	bool DxObject::Release()
	{
		RELEASE(m_pVertexBuffer);
		RELEASE(m_pIndexBuffer);
		RELEASE(m_pConstantBuffer);
		RELEASE(m_pInputLayout);
		RELEASE(m_pVertexShader);
		RELEASE(m_pPixelShader);
		return true;
	}
}