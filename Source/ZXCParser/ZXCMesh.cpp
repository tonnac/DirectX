#include "ZXCMesh.h"
#include "Mesh.h"

bool ZXCMesh::Load(const std::wstring& name)
{
	ZXCParser parser;
	if (parser.LoadZXC(name, this))
	{
		return true;
	}
	return false;
}

std::unique_ptr<Mesh> ZXCMesh::Convert(ID3D11Device* device)
{
	ZXCConverter convert;
	return std::move(convert.Convert0(this, device));
}

