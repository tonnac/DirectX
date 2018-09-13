#pragma once
#include "Core.h"
#include "Object.h"

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
	Object						m_Object;
};