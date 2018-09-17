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
private:
	PlaneObj					m_Object;
	TriangleObj					m_Triangle;
};