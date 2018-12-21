#pragma once
#include <sstream>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include <array>
#include <Windows.h>
#include <memory>
#include <functional>
#include <map>
#include <DirectXMath.h>


class ZXCMesh;
//struct AseConverter;
struct MaterialList;
struct GeomMesh;
struct Helper;

class ZXCParser
{
public:
	ZXCParser() = default;

public:
	bool LoadZXC(const std::wstring& FileName, ZXCMesh* mesh);

private:
	void LoadScene();
	void LoadMaterial();
	void LoadGeomesh();
	void LoadHelperObject();

private:
	void InputMatrix(DirectX::XMFLOAT4X4& m, size_t& index);
	void InputAnimation(Helper* h, size_t& index, int posTracknum, int rotTrackNum, int scaleTrackNum);

private:
	std::vector<std::wstring> m_Stream;

	ZXCMesh * m_zxcMesh = nullptr;

	size_t m_ScnenIndex = 0;
	size_t m_MaterialIndex = 0;

	size_t m_GeomeshIndex = 0;
	size_t m_HelperObjIndex = 0;

	static const std::array<std::wstring, 4> m_Type;
};