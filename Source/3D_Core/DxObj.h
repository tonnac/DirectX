#pragma once


#include "Define.h"


namespace DX
{
	ID3D11Buffer* CreateVertexBuffer(
		ID3D11Device* pDevice,
		UINT iNumVertex,
		UINT iVertexSize,
		LPVOID pData,
		ID3D11Buffer** ppBuffer);

	ID3D11Buffer* CreateIndexBuffer(
		ID3D11Device* pDevice,
		UINT iNumCount,
		UINT iIndexSize,
		LPVOID pData,
		ID3D11Buffer** ppBuffer);

	ID3D11Buffer* CreateConstantBuffer(
		ID3D11Device* pDevice,
		UINT iNumCount,
		UINT iIndexSize,
		ID3D11Buffer** ppBuffer,
		LPVOID pData = nullptr,
		bool bDynamic = false);

	ID3D11InputLayout* CreateInputLayout(
		ID3D11Device* pDevice,
		DWORD dwSize,
		LPCVOID lpData,
		D3D11_INPUT_ELEMENT_DESC* layout,
		UINT numElements,
		ID3D11InputLayout ** ppInputLayout);

	ID3D11VertexShader* LoadVertexShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = nullptr,
		const char * pFuncName = "VS");

	ID3D11PixelShader* LoadPixelShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = nullptr,
		const char * pFuncName = "PS");

	ID3D11GeometryShader* LoadGeometryShaderFile(
		ID3D11Device* pDevice,
		const void* pShaderFile,
		ID3DBlob** ppBlobOut = nullptr,
		const char * pFuncName = "GS");

	HRESULT CompileShaderFromFile(
		const WCHAR* szFileName,
		LPCSTR szEntryPoint,
		LPCSTR szShaderModel,
		ID3DBlob** ppBlobOut);

	ID3D11ShaderResourceView* CreateShaderResourceView(ID3D11Device* pDevice, const std::tstring& filepath);

	class DxObj
	{
	public:
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pVertexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pIndexBuffer = nullptr;
		Microsoft::WRL::ComPtr<ID3D11Buffer> m_pConstantBuffer = nullptr;
				
		Microsoft::WRL::ComPtr<ID3D11InputLayout> m_pInputLayout = nullptr;
		Microsoft::WRL::ComPtr<ID3D11VertexShader> m_pVertexShader = nullptr;
		Microsoft::WRL::ComPtr<ID3D11PixelShader> m_pPixelShader = nullptr;
		
		Microsoft::WRL::ComPtr<ID3D11GeometryShader> m_pGeometryShader = nullptr;
		Microsoft::WRL::ComPtr<ID3DBlob> m_pVSBlob = nullptr;

		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> m_pTextureSRV = nullptr;

		UINT m_iNumIndex  = 0;
		UINT m_iNumVertex = 0;
	public:
		bool PreRender(ID3D11DeviceContext* pContext,
			UINT iVertexSize);
		bool PostRender(
			ID3D11DeviceContext* pContext,
			UINT iVertexSize, UINT iCount);
		bool Render(
			ID3D11DeviceContext* pContext,
			UINT iVertexSize, UINT iCount);
		bool Release();
	public:
		DxObj();
		virtual ~DxObj();
	};
}