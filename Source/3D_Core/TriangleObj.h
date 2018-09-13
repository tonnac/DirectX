#pragma once
#include "Object.h"

class TriangleObj : public Object
{
public:
	TriangleObj() = default;
	~TriangleObj() override = default;
public:
	HRESULT	CreateVertexBuffer(ID3D11Device* pDevice) override;
	HRESULT	CreateIndexBuffer(ID3D11Device* pDevice) override;
	bool	PostRender(ID3D11DeviceContext* pContext) override;
private:

};