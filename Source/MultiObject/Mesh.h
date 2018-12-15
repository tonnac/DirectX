#pragma once

#include "Shape.h"
#include "AseMesh.h"
#include <unordered_map>
#include <queue>

class Mesh : public Shape
{
public:

	virtual HRESULT CreateVertexData()override;
	virtual HRESULT CreateIndexData()override;
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render(ID3D11DeviceContext* pContext)override;

	void InitMatrix();
	bool UpdateAnimaitions();
public:
	sScene m_Scene;
	float m_fElapsedTime = 0.0f;
	float m_fTickSpeed = 1.0f;

	std::string Name;
	std::string ParentName;

	int m_iNumFaces = 0;
	std::vector<PNCT_VERTEX> m_tmpVertexList;

	std::vector<AniTrack> m_Position;
	std::vector<AniTrack> m_Rotation;
	std::vector<AniTrack> m_Scale;

	int m_TrackIndex = 0;
	ObjectType m_Type = ObjectType::BONE;

	D3DXMATRIX InvWorld;
	D3DXMATRIX Translation;
	D3DXMATRIX Rotation;
	D3DXMATRIX Scale;

	std::vector<std::unique_ptr<Mesh>> m_NodeList;
	std::vector<std::unique_ptr<Mesh>> m_ObjectList;
	std::vector<Mesh*> m_ChildList;
	std::vector<Mesh*> m_RenderList;
	Mesh* m_Parent = nullptr;
	DirectX::BoundingBox m_BoundingBox;
};


struct AseConverter
{
	std::unique_ptr<Mesh> Convert(AseMesh* aseMesh, ID3D11Device* device);
	std::unique_ptr<Mesh> Convert0(AseMesh* aseMesh, ID3D11Device* device);
};