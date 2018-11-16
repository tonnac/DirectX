#pragma once
#include "Core.h"
#include "Shape.h"
#include "Camera.h"

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

	void		DeleteResources()override;
	HRESULT		CreateResources(const UINT& Width, const UINT& Height)override;
private:
	BoxShape	m_boxObj;
	BoxShape	m_boxObj2;
	BoxShape	m_boxObj3;
	LineShape	m_line;
	PlaneShape	m_plane;

};
