#pragma once
#include "Core.h"
#include "Map.h"
#include "Frustum.h"
#include "LightObj.h"

#define MaxLights 16

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool	   	Init() override;
	bool	   	Frame() override;
	bool	   	Render() override;
	bool	   	Release() override;
private:
	LightObj	m_SpotLightObj;

	Map			m_Map;
};
