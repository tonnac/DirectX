
#include "Map.h"

bool Map::CreateMap(const MapDesc& desc)
{
	m_iNumRows = desc.iNumRows;
	m_iNumCols = desc.iNumCols;
	m_iNumCellRow = m_iNumRows - 1;
	m_iNumCellCols = m_iNumCols - 1;
	m_iNumFace = m_iNumCellRow * m_iNumCellCols * 2;
	m_iNumVertices = m_iNumRows * m_iNumCols;
	m_fDistanceOffset = desc.fDistance;
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
	m_DxObject.m_iNumVertex = m_VertexList.size();
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
	m_DxObject.m_iNumIndex = m_IndexList.size();

	return hr;
}
		
float Map::GetHeightOfVertex(int iIndex)
{
	return 0.0f;
}

D3DXVECTOR2 Map::GetTextureOfVertex(float fU, float fV)
{
	return D3DXVECTOR2(fU, fV);
}

D3DXVECTOR4 Map::GetColorOfVertex(int iIndex)
{
	return D3DXVECTOR4(1.0f, 1.0f, 1.0f, 1.0f);
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