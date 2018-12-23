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

	return false;
}

bool Mesh::Render(ID3D11DeviceContext * pContext)
{
	for (auto&x : m_RenderList)
	{
		pContext->VSSetConstantBuffers(0, 1, &x->ConstantBuffer);
		pContext->PSSetShaderResources(0, 1, &x->ShaderResourceView);
		pContext->DrawIndexedInstanced(x->Subgeo.IndexCount, 1, x->Subgeo.StartIndexLocation, x->Subgeo.BaseVertexLocation, 0);
	}
	return true;
}

void Mesh::SetMatrix(D3DXMATRIX * pWorld, D3DXMATRIX * pView, D3DXMATRIX * pProj)
{
}

void Mesh::InitMatrix()
{
}

bool Mesh::UpdateAnimaitions()
{
	return true;
}

int BoneAnimation::GetStartTime() const
{
	return 0;
}

int BoneAnimation::GetEndTime() const
{
	return 0;
}

void BoneAnimation::Interpoloate(float t, DirectX::XMFLOAT4X4 & M) const
{
}

int AnimationClip::GetClipStartTime() const
{
	return 0;
}

int AnimationClip::GetClipEndTime() const
{
	return 0;
}

void AnimationClip::Interpoloate(float t, std::vector<DirectX::XMFLOAT4X4>& boneTransforms) const
{
}

UINT SkinnedData::BoneCount() const
{
	return 0;
}

int SkinnedData::GetClipStartTime(const std::string & clipName) const
{
	return 0;
}

int SkinnedData::GetClipEndTime(const std::string & clipName) const
{
	return 0;
}

void SkinnedData::Set(std::vector<int>& boneHierarchy, std::vector<DirectX::XMFLOAT4X4>& boneOffsets, std::unordered_map<std::string, AnimationClip>& animations)
{
}

void SkinnedData::GetFinalTransforms(const std::string & clipName, int timePos, std::vector<DirectX::XMFLOAT4X4>& finalTransforms) const
{
}
