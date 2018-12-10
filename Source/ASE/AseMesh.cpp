#include "AseMesh.h"

bool AseMesh::LoadScene()
{
	_fgetts(m_pBuffer, 256, m_pStream);


	return false;
}

bool AseMesh::Load(std::tstring name)
{
	m_pStream = _tfopen(name.c_str(), L"rt");
	if (m_pStream == nullptr) return false;


	while (!feof(m_pStream))
	{
		_fgetts(m_pBuffer, 256, m_pStream);
		_stscanf(m_pBuffer, L"%s", m_pString);
	}

	return true;
}
