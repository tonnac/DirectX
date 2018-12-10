#pragma once
#include "Core.h"
#include "Map.h"
#include "Frustum.h"
#include "LightObj.h"
#include "AseMesh.h"

#define MaxLights 16

struct A
{
	int m_a;

};


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



	std::shared_ptr<A> m_s;
	A* m_a;

	AseMesh m_obj;
};
