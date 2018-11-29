#pragma once
#include "Core.h"
#include "Map.h"
#include "Frustum.h"


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
	LineShape m_Line;

	Map			m_Map;
};
