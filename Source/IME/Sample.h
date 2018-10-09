#pragma once
#include "Core.h"
#include "PlaneObj.h"
#include "TriangleObj.h"

#pragma comment(lib, "imm32")

class Sample : public Core
{
public:
	Sample();
	~Sample();
public:
	bool			Init	() override;
	bool			Frame	() override;
	bool			Render	() override;
	bool			Release	() override;
	LRESULT			WndProc	(HWND hwnd, UINT msg, WPARAM wparam, LPARAM lparam);
	void			ResetCompositionString();
private:
	HIMC			m_hImc;
	std::tstring	m_Compstring;
	std::tstring	m_CompWord;
	std::tstring	m_CurrentString;

	std::vector<std::tstring> m_stringList;

	std::tstring	m_Conversion;
	DWORD			m_dwConversion;
	INT				m_nComSize;
};