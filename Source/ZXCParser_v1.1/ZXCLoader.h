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
#include "Mesh.h"

class ZXCLoader
{
public:
	ZXCLoader() = default;

public:
	bool LoadZXC(
		const std::wstring& FileName, 
		Mesh * mesh);

private:
	void LoadScene(std::wifstream& fp);
	void LoadMaterial(std::wifstream& fp, UINT numMaterials);
	void LoadMesh(std::wifstream& fp, UINT numMeshes);
	void LoadHelper(std::wifstream& fp, UINT numHelpers);
	void ReadVertex(std::wifstream& fp, UINT numVertices);
	void ReadIndex(std::wifstream& fp, UINT numIndices);

private:
	void InputMatrix(DirectX::XMFLOAT4X4& m, size_t& index);
//	void InputAnimation(Helper* h, size_t& index, int posTracknum, int rotTrackNum, int scaleTrackNum);

private:
	std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>> mSubsetID;
	std::map<std::pair<UINT, UINT>, std::vector<std::pair<UINT, std::wstring>>> mMaterials;
	Mesh * mMesh = nullptr;
};