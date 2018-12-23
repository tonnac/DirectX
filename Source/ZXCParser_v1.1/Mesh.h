#pragma once

#include "Define.h"
#include "Shape.h"
#include <DirectXCollision.h>
#include <DirectXMath.h>

struct KeyFrame
{
	KeyFrame() {}
	~KeyFrame() {}

	int Tick;
	DirectX::XMFLOAT3 Translations;
	DirectX::XMFLOAT3 Scale;
	DirectX::XMFLOAT4 ScaleQuat;
	DirectX::XMFLOAT4 RotationQuat;
};

struct BoneAnimation
{
	int GetStartTime()const;
	int GetEndTime()const;

	void Interpoloate(float t, DirectX::XMFLOAT4X4& M)const;

	std::vector<KeyFrame> Keyframes;
};

struct AnimationClip
{
	int GetClipStartTime()const;
	int GetClipEndTime()const;

	void Interpoloate(float t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms)const;

	std::vector<BoneAnimation> BoneAnimations;
};

class SkinnedData
{
public:

	UINT BoneCount()const;

	int GetClipStartTime(const std::string& clipName)const;
	int GetClipEndTime(const std::string& clipName)const;

	void Set(
		std::vector<int>& boneHierarchy,
		std::vector<DirectX::XMFLOAT4X4>& boneOffsets,
		std::unordered_map<std::string, AnimationClip>& animations);

	void GetFinalTransforms(const std::string& clipName, int timePos,
		std::vector<DirectX::XMFLOAT4X4>& finalTransforms)const;

private:
	std::vector<int> mBoneHierarchy;

	std::vector<DirectX::XMFLOAT4X4> mBoneOffsets;

	std::unordered_map<std::string, AnimationClip> mAnimations;
};

struct SkinnedModelInstance
{
	SkinnedData* SkinnedInfo = nullptr;
	std::vector<DirectX::XMFLOAT4X4> FinalTransforms;
	std::string ClipName;
	int TimePos = 0;

	void UpdateSkinnedAnimation(int dt)
	{
		TimePos += dt;

		if (TimePos > SkinnedInfo->GetClipEndTime(ClipName))
			TimePos = 0.0f;

		SkinnedInfo->GetFinalTransforms(ClipName, TimePos, FinalTransforms);
	}
};

struct Subset
{
	UINT VertexStart;
	UINT VertexCount;
	UINT FaceStart;
	UINT FaceCount;
};

struct SubmeshGeometry
{
	UINT IndexCount = 0;
	UINT StartIndexLocation = 0;
	INT BaseVertexLocation = 0;

	DirectX::BoundingBox Bounds;
};

struct RenderItem
{
	SubmeshGeometry Subgeo;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> ShaderResourceView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Buffer> ConstantBuffer = nullptr;
};

class Mesh : public Shape
{
public:

	virtual HRESULT CreateVertexData()override;
	virtual HRESULT CreateIndexData()override;
	virtual bool Init()override;
	virtual bool Frame()override;
	virtual bool Render(ID3D11DeviceContext* pContext)override;

	virtual void SetMatrix(D3DXMATRIX* pWorld = nullptr, D3DXMATRIX* pView = nullptr, D3DXMATRIX* pProj = nullptr)override;

	void InitMatrix();
	bool UpdateAnimaitions();
public:
	std::string Name;
	std::string ParentName;

	SkinnedModelInstance * SkinnedModelInst = nullptr;

	D3DXMATRIX InvWorld;
	D3DXVECTOR3 pos;
	D3DXQUATERNION rot;
	D3DXQUATERNION scaleaxis;
	D3DXVECTOR3 scale;

	std::vector<std::unique_ptr<Mesh>> m_NodeList;
	std::vector<std::unique_ptr<Mesh>> m_Submesh;
	std::vector<std::unique_ptr<RenderItem>> m_RenderList;
	Mesh* m_Parent = nullptr;
};