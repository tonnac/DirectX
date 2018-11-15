#include "Shape.h"

LineShape::LineShape()
{
	m_Primitive = D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST;
}
LineShape::~LineShape()
{

}

//HRESULT LineShape::CreateVertexData()
//{
//
//	return S_OK;
//}

HRESULT LineShape::CreateIndexData()
{
	m_IndexList.resize(6);
	int iIndex = 0;
	m_IndexList[iIndex++] = 0; 	m_IndexList[iIndex++] = 1;

	m_DxObject.m_iNumIndex = (UINT)m_IndexList.size();
	return S_OK;
}