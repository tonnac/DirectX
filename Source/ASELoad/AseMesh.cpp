#include "AseMesh.h"


void AseMesh::Load(const std::wstring& name)
{
	AseParser parser;
	parser.LoadAse(name, this);
}

