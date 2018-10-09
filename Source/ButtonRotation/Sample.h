#pragma once
#include "Core.h"
#include "PlaneObj.h"
#include "TriangleObj.h"

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool						Init					() override;
	bool						Frame					() override;
	bool						Render					() override;
	bool						Release					() override;
	void						CreateRasterizerState();
private:
	ID3D11RasterizerState *		m_pWFRasterizer;
	ID3D11RasterizerState *		m_pRasterizer;
	PlaneObj					m_Object;
	TriangleObj					m_Triangle;
};