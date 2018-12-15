#include "AseMesh.h"
#include "Mesh.h"

bool AseMesh::Load(const std::wstring& name)
{
	AseParser parser;
	if (parser.LoadAse(name, this))
	{
		return true;
	}
	return false;
}

std::unique_ptr<Mesh> AseMesh::Convert(ID3D11Device* device)
{
	AseConverter convert;
	return std::move(convert.Convert0(this, device));
}

