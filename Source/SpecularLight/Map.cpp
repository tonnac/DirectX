#include "Map.h"

using Microsoft::WRL::ComPtr;

bool Map::CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext, const std::tstring& szName)
{
	ComPtr<ID3D11Resource> pLoadTexture = nullptr;
	D3DX11_IMAGE_LOAD_INFO info;
	D3DX11_IMAGE_INFO srcInfo;
	ZeroMemory(&info, sizeof(D3DX11_IMAGE_LOAD_INFO));
	info.Usage = D3D11_USAGE_STAGING;
	info.CpuAccessFlags = D3D11_CPU_ACCESS_WRITE | D3D11_CPU_ACCESS_READ;
	info.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	info.MipLevels = 1;
	info.pSrcInfo = &srcInfo;

	if (FAILED(D3DX11CreateTextureFromFile(pDevice,
		szName.c_str(),
		&info, nullptr,
		pLoadTexture.GetAddressOf(),
		nullptr)))
	{
		return false;
	}

	ComPtr<ID3D11Texture2D> pTex2D = nullptr;
	if (FAILED(pLoadTexture->QueryInterface(IID_PPV_ARGS(pTex2D.GetAddressOf()))))
	{
		return false;
	}

	D3D11_TEXTURE2D_DESC desc;
	pTex2D->GetDesc(&desc);

	m_fHeightList.resize(desc.Height * desc.Width);

	D3D11_MAPPED_SUBRESOURCE mapSubresource;
	if (SUCCEEDED(pContext->Map(pTex2D.Get(),
		D3D11CalcSubresource(0, 0, 1),
		D3D11_MAP_READ, 0, 
		&mapSubresource)))
	{
		UCHAR* pTexels = (UCHAR*)mapSubresource.pData;

		for (UINT iRow = 0; iRow < desc.Height; ++iRow)
		{
			UINT rowStart = iRow * mapSubresource.RowPitch;
			for (UINT iCol = 0; iCol < desc.Width; ++iCol)
			{
				UINT colStart = iCol * 4;
				UINT uRed = pTexels[rowStart + colStart + 0];
				if (iRow * desc.Width + iCol >= 512)
				{
					int a = 5;
				}
				m_fHeightList[iRow * desc.Width + iCol] = (float)uRed;
			}
		}

		pContext->Unmap(pTex2D.Get(), D3D11CalcSubresource(0, 0, 1));
	}

	m_iNumCols = desc.Width;
	m_iNumRows = desc.Height;

	return true;
}

bool Map::CreateMap(const MapDesc& desc)
{
	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellRow = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumFace = m_iNumCellRow * m_iNumCellCols * 2;
	m_iNumVertices = m_iNumRows * m_iNumCols;
	m_fDistanceOffset = desc.fDistance;
	m_fScaleHeight = desc.fScaleHeight;
	return true;
}

void Map::Load(ID3D11Device* pDevice, const MapDesc& desc)
{
	if (!CreateMap(desc))
		return;

	Shape::Create(pDevice, desc.szShaderFile, desc.szTextureFile);
}

HRESULT Map::CreateVertexData()
{
	HRESULT hr = S_OK;

	m_VertexList.resize(m_iNumVertices);

	float fHalfCols = (m_iNumCols - 1) * 0.5f;
	float fHalfRows = (m_iNumRows - 1) * 0.5f;

	float fOffSetU = 1.0f / (m_iNumCols - 1);
	float fOffSetV = 1.0f / (m_iNumRows - 1);

	for (int iRow = 0; iRow < m_iNumRows; ++iRow)
	{
		for (int iCol = 0; iCol < m_iNumCols; ++iCol)
		{
			int iIndex = iRow * m_iNumCols + iCol;
			m_VertexList[iIndex].p.x = (iCol - fHalfCols) * m_fDistanceOffset;
			m_VertexList[iIndex].p.y = GetHeightOfVertex(iIndex);
			m_VertexList[iIndex].p.z = -(iRow - fHalfRows) * m_fDistanceOffset;

			m_VertexList[iIndex].n = GetNormalOfVertex(iIndex);
			m_VertexList[iIndex].c = GetColorOfVertex(iIndex);
			m_VertexList[iIndex].t = GetTextureOfVertex(fOffSetU * iCol, fOffSetV * iRow);
		}
	}
	m_DxObject.m_iNumVertex = (UINT)m_VertexList.size();
	m_DxObject.m_iVertexSize = sizeof(PNCT_VERTEX);

	return hr;
}

HRESULT Map::CreateIndexData()
{
	HRESULT hr = S_OK;

	m_IndexList.resize(m_iNumFace * 3);

	float fHalfCols = (m_iNumCols - 1) * 0.5f;
	float fHalfRows = (m_iNumRows - 1) * 0.5f;

	float fOffSetU = 1.0f / (m_iNumCols - 1);
	float fOffSetV = 1.0f / (m_iNumRows - 1);

	int iIndex = 0;

	for (int iRow = 0; iRow < m_iNumCellRow; ++iRow)
	{
		for (int iCol = 0; iCol < m_iNumCellCols; ++iCol)
		{
			int iNextRow = iRow + 1;
			m_IndexList[iIndex + 0] = iRow * m_iNumCols + iCol;
			m_IndexList[iIndex + 1] = m_IndexList[iIndex + 0] + 1;
			m_IndexList[iIndex + 2] = iNextRow * m_iNumCols + iCol;

			m_IndexList[iIndex + 3] = m_IndexList[iIndex + 2];
			m_IndexList[iIndex + 4] = m_IndexList[iIndex + 1];
			m_IndexList[iIndex + 5] = m_IndexList[iIndex + 2] + 1;
			iIndex += 6;
		}
	}
	m_DxObject.m_iNumIndex = (UINT)m_IndexList.size();

	GenVertexNormal();

	return hr;
}

float Map::GetHeightOfVertex(int iIndex)
{
	if (!m_fHeightList.empty())
		return m_fHeightList[iIndex] * m_fScaleHeight;
	else
		return 0.0f;
}

D3DXVECTOR2 Map::GetTextureOfVertex(float fU, float fV)
{
	return D3DXVECTOR2(fU, fV);
}

D3DXVECTOR4 Map::GetColorOfVertex(int iIndex)
{
	return D3DXVECTOR4(randstep(0.0f, 1.0f), randstep(0.0f, 1.0f), randstep(0.0f, 1.0f), 1.0f);
}

D3DXVECTOR3 Map::GetNormalOfVertex(int iIndex)
{
	return D3DXVECTOR3(0.0f, 1.0f, 0.0f);
}



bool Map::Init()
{
	return true;
}

bool Map::Frame()
{
	return true;
}

bool Map::Render(ID3D11DeviceContext* pContext)
{
	Shape::Render(pContext);
	return true;
}

bool Map::Release()
{
	return true;
}

bool Map::GenVertexNormal()
{
	InitFaceNormals();
	GenNormalLookupTable();
	CalcPerVertexNormalsFastLookup();
	return true;
}

void Map::InitFaceNormals()
{
	m_FaceNormalList.resize(m_iNumFace);
}

void Map::CalcFaceNormals()
{
	int iFaceIndex = 0;

	for (int i = 0; i < m_IndexList.size(); i+=3)
	{
		DWORD i0 = m_IndexList[i];
		DWORD i1 = m_IndexList[i+1];
		DWORD i2 = m_IndexList[i+2];

		m_FaceNormalList[iFaceIndex++] = ComputeFaceNormal(i0, i1, i2);
	}
}

D3DXVECTOR3 Map::ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2)
{
	D3DXVECTOR3 vNormal;
	D3DXVECTOR3 v0 = m_VertexList[i1].p - m_VertexList[i0].p;
	D3DXVECTOR3 v1 = m_VertexList[i2].p - m_VertexList[i0].p;

	D3DXVec3Cross(&vNormal, &v0, &v1);
	D3DXVec3Normalize(&vNormal, &vNormal);
	return vNormal;
}

void Map::GenNormalLookupTable()
{
	m_NormalLookupTable.resize(m_iNumVertices);
	for (int iFace = 0; iFace < m_iNumFace; ++iFace)
	{
		for (int iVertex = 0; iVertex < 3; ++iVertex)
		{
			int iIndex = m_IndexList[iFace * 3 + iVertex];
			for (int iTable = 0; iTable < 6; ++iTable)
			{
				if (m_NormalLookupTable[iIndex].i[iTable] == -1)
				{
					m_NormalLookupTable[iIndex].i[iTable] = iFace;
					break;
				}
			}
		}
	}
}

void Map::CalcPerVertexNormalsFastLookup()
{
	CalcFaceNormals();
	for (int iVertex = 0; iVertex < m_NormalLookupTable.size(); ++iVertex)
	{
		D3DXVECTOR3 avgNormal(0, 0, 0);
		for (int iFace = 0; iFace < m_NormalLookupTable[iVertex].i.size(); ++iFace)
		{
			int iFaceIndex = m_NormalLookupTable[iVertex].i[iFace];
			if (iFaceIndex != -1)
			{
				avgNormal += m_FaceNormalList[iFaceIndex];
			}
		}
		D3DXVec3Normalize(&m_VertexList[iVertex].n, &avgNormal);
	}
}