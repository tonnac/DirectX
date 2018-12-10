#pragma once

#include "Shape.h"

struct MapDesc
{
	int iNumCols;
	int iNumRows;
	float fDistance;
	float fScaleHeight;
	const std::tstring szTextureFile;
	const std::tstring szShaderFile;
};

struct Index
{
	DWORD i0;
	DWORD i1;
	DWORD i2;
};

struct IndexTable
{
	std::array<int, 6> i;
	IndexTable()
	{
		for (auto&x : i)
		{
			x = -1;
		}
	}
};

class Map : public Shape
{
public:
	MapDesc m_Mapdesc;
	int m_iNumRows;
	int m_iNumCols;
	int m_iNumCellRow;
	int m_iNumCellCols;
	int m_iNumFace;
	int m_iNumVertices;
	float m_fDistanceOffset;
	float m_fScaleHeight = 1.0f;
	std::vector<float> m_fHeightList;
	std::vector<D3DXVECTOR3> m_FaceNormalList;
	std::vector<IndexTable> m_NormalLookupTable;

public:
	bool CreateHeightMap(ID3D11Device* pDevice, ID3D11DeviceContext* pContext ,const std::tstring& szName);
	bool CreateMap(const MapDesc& desc);
	void Load(ID3D11Device* pDevice, const MapDesc& desc);
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();


	float GetHeight(float fPosX, float fPosY);
	bool GenVertexNormal();
	void InitFaceNormals();
	void CalcFaceNormals();
	D3DXVECTOR3 ComputeFaceNormal(DWORD i0, DWORD i1, DWORD i2);
	void GenNormalLookupTable();
	void CalcPerVertexNormalsFastLookup();


	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;

	float GetHeightOfVertex(int iIndex);
	D3DXVECTOR2 GetTextureOfVertex(float fU, float fV);
	D3DXVECTOR4 GetColorOfVertex(int iIndex);
	D3DXVECTOR3 GetNormalOfVertex(int iIndex);
};