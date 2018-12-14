#include "AseMesh.h"


bool AseMesh::Load(const std::wstring& name)
{
	AseParser parser;
	if (parser.LoadAse(name, this))
	{
		return true;
	}
	return false;
}

