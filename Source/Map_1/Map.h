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

public:
	bool CreateMap(const MapDesc& desc);
	void Load(ID3D11Device* pDevice, const MapDesc& desc);
	bool Init();
	bool Frame();
	bool Render(ID3D11DeviceContext* pContext);
	bool Release();

	HRESULT CreateVertexData()override;
	HRESULT CreateIndexData()override;

	float GetHeightOfVertex(int iIndex);
	D3DXVECTOR2 GetTextureOfVertex(float fU, float fV);
	D3DXVECTOR4 GetColorOfVertex(int iIndex);
	D3DXVECTOR3 GetNormalOfVertex(int iIndex);
};