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

	if (!m_Position.empty() || !m_Rotation.empty() || !m_Scale.empty())
	{
		if (m_fElapsedTime >= m_Scene.LastFrame * m_Scene.TicksperFrame)
		{
			m_fElapsedTime = (float)m_Scene.FirstFrame * m_Scene.TicksperFrame;
			m_PosTrackIndex = m_RotTrackIndex = m_ScaleTrackIndex = 0;
		}

		UpdateAnimaitions();
	}

	if (m_Parent != nullptr)
		m_matWorld = m_matWorld * m_Parent->m_matWorld;

	D3DXVECTOR3 v0, v1, v2, v3;
	v0 = m_matWorld.m[0];
	v1 = m_matWorld.m[1];
	v2 = m_matWorld.m[2];
	D3DXVec3Cross(&v3, &v1, &v2);

	if (D3DXVec3Dot(&v3, &v0) < 0.0f)
	{
		D3DXMATRIX matW;
		D3DXMatrixScaling(&matW, -1.0f, -1.0f, -1.0f);
		D3DXMatrixMultiply(&m_matWorld, &m_matWorld, &matW);
	}

	D3DXMatrixInverse(&InvWorld, nullptr, &m_matWorld);

	if (!m_ChildList.empty())
	{
		for (auto&x : m_ChildList)
		{
			x->Frame();
		}
	}

	return false;
}

bool Mesh::Render(ID3D11DeviceContext * pContext)
{
	if (!m_RenderList.empty())
	{
		for (auto&x : m_RenderList)
		{
			x->Render(pContext);
		}
	}
	if (!m_ObjectList.empty())
	{
		for (auto & x : m_ObjectList)
		{
			x->Render(pContext);
		}
	}
	if (m_DxObject.m_pVertexBuffer != nullptr)
	{
		Shape::Render(pContext);
	}
	return true;
}

void Mesh::SetMatrix(D3DXMATRIX * pWorld, D3DXMATRIX * pView, D3DXMATRIX * pProj)
{
	if (!m_RenderList.empty())
	{
		for (auto&x : m_RenderList)
		{
			x->SetMatrix(pWorld, pView, pProj);
		}
	}
	if (!m_ObjectList.empty())
	{
		for (auto & x : m_ObjectList)
		{
			x->SetMatrix(&m_matWorld, pView, pProj);
		}
	}
	if (m_DxObject.m_pVertexBuffer != nullptr)
	{
		Shape::SetMatrix(pWorld, pView, pProj);
	}
}

void Mesh::InitMatrix()
{
	D3DXMATRIX Inverse;
	D3DXQUATERNION qR;
	D3DXVECTOR3 vTrans, vScale;

	Inverse = m_matWorld;

	if (m_Parent != nullptr)
	{
		Inverse *= m_Parent->InvWorld;
	}

	D3DXMatrixDecompose(&vScale, &qR, &vTrans, &Inverse);
	D3DXMatrixScaling(&Scale, vScale.x, vScale.y, vScale.z);
	D3DXMatrixTranslation(&Translation, vTrans.x, vTrans.y, vTrans.z);
	D3DXMatrixRotationQuaternion(&Rotation, &qR);

	if (m_Rotation.empty() || m_Rotation.front().mTick != 0)
	{
		std::vector<AniTrack> pe;
		AniTrack track;
		track.mQuatRotation.x = qR.x;
		track.mQuatRotation.y = qR.y;
		track.mQuatRotation.z = qR.z;
		track.mQuatRotation.w = qR.w;
		pe.push_back(track);
		pe.insert(pe.end(), m_Rotation.begin(), m_Rotation.end());
		m_Rotation = pe;
	}

	if (m_Position.empty() || m_Position.front().mTick != 0)
	{
		std::vector<AniTrack> pe;
		AniTrack track;
		track.mPosition.x = vTrans.x;
		track.mPosition.y = vTrans.y;
		track.mPosition.z = vTrans.z;
		pe.push_back(track);
		pe.insert(pe.end(), m_Position.begin(), m_Position.end());
		m_Position = pe;
	}

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
	if (!m_Position.empty())
	{
		D3DXVECTOR3 lerpPos;

		if (m_fElapsedTime <= m_Position.front().mTick)
		{
			lerpPos = (float*)&m_Position.front().mPosition;
		}
		else if (m_fElapsedTime >= m_Position.back().mTick)
		{
			lerpPos = (float*)&m_Position.back().mPosition;
		}
		else
		{
			for (UINT i = 0; i < (UINT)m_Position.size(); ++i)
			{
				if (m_fElapsedTime >= m_Position[i].mTick && m_fElapsedTime <= m_Position[i + 1].mTick)
				{
					D3DXVECTOR3 prevPos = (float*)&m_Position[i].mPosition;
					D3DXVECTOR3 nextPos = (float*)&m_Position[i+1].mPosition;

					float fRatio = (m_fElapsedTime - (float)m_Position[i].mTick) / (float)(m_Position[i+1].mTick - m_Position[i].mTick);

					D3DXVec3Lerp(&lerpPos, &prevPos, &nextPos, fRatio);

					break;
				}
			}
		}

		D3DXMatrixTranslation(&Translation, lerpPos.x, lerpPos.y, lerpPos.z);
	}

	if (!m_Rotation.empty())
	{
		D3DXQUATERNION lerpRot;

		if (m_fElapsedTime <= m_Rotation.front().mTick)
		{
			lerpRot = (float*)&m_Rotation.front().mQuatRotation;
		}
		else if (m_fElapsedTime >= m_Rotation.back().mTick)
		{
			lerpRot = (float*)&m_Rotation.back().mQuatRotation;
		}
		else
		{
			for (UINT i = 0; i < (UINT)m_Rotation.size(); ++i)
			{
				if (m_fElapsedTime >= m_Rotation[i].mTick && m_fElapsedTime <= m_Rotation[i + 1].mTick)
				{
					D3DXQUATERNION prevRot = (float*)&m_Rotation[i].mQuatRotation;
					D3DXQUATERNION nextRot = (float*)&m_Rotation[i + 1].mQuatRotation;

					float fRatio = (m_fElapsedTime - (float)m_Rotation[i].mTick) / (float)(m_Rotation[i + 1].mTick - m_Rotation[i].mTick);

					D3DXQuaternionSlerp(&lerpRot, &prevRot, &nextRot, fRatio);

					break;
				}
			}
		}

		D3DXMatrixRotationQuaternion(&Rotation, &lerpRot);
	}

	if (!m_Scale.empty())
	{
		D3DXMATRIX scaleRot;
		D3DXMATRIX scaleInv;
		D3DXMATRIX scale0;

		D3DXVECTOR3 scaleSize;
		D3DXQUATERNION lerpRot;

		if (m_fElapsedTime <= m_Scale.front().mTick)
		{
			lerpRot = (float*)&m_Scale.front().mQuatRotation;
			scaleSize = (float*)&m_Scale.front().mPosition;
		}
		else if (m_fElapsedTime >= m_Scale.back().mTick)
		{
			lerpRot = (float*)&m_Scale.back().mQuatRotation;
			scaleSize = (float*)&m_Scale.back().mPosition;
		}
		else
		{
			for (UINT i = 0; i < (UINT)m_Scale.size(); ++i)
			{
				if (m_fElapsedTime >= m_Scale[i].mTick && m_fElapsedTime <= m_Scale[i + 1].mTick)
				{
					D3DXQUATERNION prevRot = (float*)&m_Scale[i].mQuatRotation;
					D3DXQUATERNION nextRot = (float*)&m_Scale[i + 1].mQuatRotation;
					D3DXVECTOR3 prevSize = (float*)&m_Scale[i].mPosition;
					D3DXVECTOR3 nextSize = (float*)&m_Scale[i + 1].mPosition;

					float fRatio = (m_fElapsedTime - m_Scale[i].mTick) / (m_Scale[i + 1].mTick - m_Scale[i].mTick);

					D3DXQuaternionSlerp(&lerpRot, &prevRot, &nextRot, fRatio);
					D3DXVec3Lerp(&scaleSize, &prevSize, &nextSize, fRatio);

					break;
				}
			}
		}

		D3DXMatrixRotationQuaternion(&scaleRot, &lerpRot);
		D3DXMatrixInverse(&scaleInv, nullptr, &scaleRot);
		D3DXMatrixScaling(&scale0, scaleSize.x, scaleSize.y, scaleSize.z);

		Scale = scaleInv * scale0 * scaleRot;
	}

	m_matWorld = Scale * Rotation * Translation;

	return true;
}
