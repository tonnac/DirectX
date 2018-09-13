#pragma once
#include "Object.h"

class PlaneObj : public Object
{
public:
	PlaneObj() = default;
	~PlaneObj() override = default;
public:
	HRESULT	CreateVertexBuffer(ID3D11Device* pDevice) override;
	HRESULT	CreateIndexBuffer(ID3D11Device* pDevice) override;
private:

};