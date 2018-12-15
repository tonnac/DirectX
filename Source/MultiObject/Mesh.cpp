#include "Mesh.h"

HRESULT Mesh::CreateVertexData()
{
	
	return S_OK;
}

HRESULT Mesh::CreateIndexData()
{

	return S_OK;
}

bool Mesh::Init()
{
	InitMatrix();
	return true;
}

bool Mesh::Frame()
{
	m_fElapsedTime += g_fSecPerFrame * m_fTickSpeed * m_Scene.FrameSpeed * m_Scene.TicksperFrame;
	if (m_fElapsedTime >= m_Scene.LastFrame * m_Scene.TicksperFrame)
	{
		m_fElapsedTime = m_Scene.FirstFrame * m_Scene.TicksperFrame;
	}

	return false;
}

bool Mesh::Render(ID3D11DeviceContext * pContext)
{
	return false;
}

void Mesh::InitMatrix()
{
	D3DXVECTOR3 trans;
	D3DXVECTOR4 rota0;
	D3DXVECTOR4 scale0;
	D3DXQUATERNION rota;
	D3DXQUATERNION scalequat;
	D3DXVECTOR3 scale;
	D3DXMATRIX scalemat;
	D3DXMATRIX scalematinv;
	D3DXMATRIX scalemat0;

	CopyMemory(&trans, &Translation.m[0], sizeof(float) * 3);
	CopyMemory(&rota0, &Rotation.m[0], sizeof(float) * 4);
	CopyMemory(&scale, &Scale.m[0], sizeof(float) * 3);
	CopyMemory(&scale0, &Scale.m[1], sizeof(float) * 4);

	D3DXQuaternionRotationAxis(&rota, &D3DXVECTOR3(rota0.x, rota0.y, rota0.z), rota0.w);
	D3DXQuaternionRotationAxis(&scalequat, &D3DXVECTOR3(scale0.x, scale0.y, scale0.z), scale0.w);

	D3DXMatrixTranslation(&Translation, trans.x, trans.y, trans.z);
	D3DXMatrixRotationQuaternion(&Rotation, &rota);
	D3DXMatrixRotationQuaternion(&scalemat, &scalequat);
	D3DXMatrixInverse(&scalematinv, nullptr, &scalemat);
	D3DXMatrixScaling(&scalemat0, scale.x, scale.y, scale.z);
	
	Scale = scalematinv * scalemat0 * scalemat;

	m_matWorld = Scale * Rotation * Translation;
	D3DXMatrixInverse(&InvWorld, nullptr, &m_matWorld);
	
	if (!m_ChildList.empty())
	{
		for (auto & x : m_ChildList)
		{
			x->InitMatrix();
		}
	}
}

bool Mesh::UpdateAnimaitions()
{
	return true;
}
