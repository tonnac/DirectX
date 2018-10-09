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
public:
	HRESULT						SetRasterizerState(const D3D11_FILL_MODE& fill = D3D11_FILL_SOLID);
private:
	PlaneObj					m_Object;
	ID3D11RasterizerState*		m_pRSSolid = nullptr;
	ID3D11RasterizerState*		m_pRSWireFrame = nullptr;
};