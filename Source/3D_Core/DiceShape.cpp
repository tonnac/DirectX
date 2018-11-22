#include "Shape.h"


HRESULT	DiceShape::CreateVertexData()
{
	BoxShape::CreateVertexData();

	//1
	m_VertexList[0].t = { 0.76f, 0.34f };
	m_VertexList[1].t = { 1.0f, 0.34f };
	m_VertexList[2].t = { 1.0f, 0.66f };
	m_VertexList[3].t = { 0.76f, 0.66f };

	//6
	m_VertexList[4].t = { 0.25f, 0.0f };
	m_VertexList[5].t = { 0.5f, 0.0f };
	m_VertexList[6].t = { 0.5f, 0.33f };
	m_VertexList[7].t = { 0.25f, 0.33f };

	//2
	m_VertexList[8].t = { 0.5f, 0.34f };
	m_VertexList[9].t = { 0.75f, 0.34f };
	m_VertexList[10].t = { 0.75f, 0.66f };
	m_VertexList[11].t = { 0.5f, 0.66f };

	//3
	m_VertexList[12].t = { 0.25f, 0.33f };
	m_VertexList[13].t = { 0.5f, 0.33f };
	m_VertexList[14].t = { 0.5f, 0.66f };
	m_VertexList[15].t = { 0.25f, 0.66f };

	//5
	m_VertexList[16].t = { 0.26f, 0.66f };
	m_VertexList[17].t = { 0.5f, 0.66f };
	m_VertexList[18].t = { 0.5f, 1.0f };
	m_VertexList[19].t = { 0.26f, 1.0f };

	//4
	m_VertexList[20].t = { 0.0f, 0.66f };
	m_VertexList[21].t = { 0.25f, 0.66f };
	m_VertexList[22].t = { 0.25f, 0.33f };
	m_VertexList[23].t = { 0.0f, 0.33f };
	//0.0f, 0.25f, 0.34f, 0.66f
	//0.0f, 0.66f
	//0.25f, 0.66f
	//0.25f, 0.33f
	//0.0f, 0.33f
	return S_OK;
}