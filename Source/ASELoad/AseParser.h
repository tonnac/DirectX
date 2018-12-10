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

class AseMesh;

class AseParser
{
public:
	AseParser() = default;

public:
	bool LoadAse(const std::wstring& FileName, AseMesh* mesh);

private:
	void LoadScene();
	void LoadMaterial();
	void LoadGeomesh();

	void InputScene(std::istringstream& is, size_t SceneTypeIndex);
	void InputMaterial(std::istringstream& is, size_t MaterialIndex, size_t MaterialType);
	void InputGeomesh(size_t& streamIndex, size_t GeomeshIndex, size_t GeomeshType);

	void InputMatrix(size_t& streamIndex, size_t GeomeshIndex, size_t GeomeshType);
	void InputVertex(size_t& streamIndex, size_t GeomeshIndex, size_t GeomeshType);

private:
	std::vector<std::string> m_Stream;

	AseMesh * m_aseMesh = nullptr;

	size_t m_ScnenIndex;
	size_t m_MaterialIndex;
	std::vector<size_t> m_GeomeshIndex;

	static const std::array<std::string, 3> m_Type;
	static const std::array<std::string, 4> m_SceneType;
	static const std::array<std::string, 3> m_MaterialType;
	static const std::array<std::string, 4> m_GeomeshType;
};