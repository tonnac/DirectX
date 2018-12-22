
#include "ZXCWriter.h"
#include "ObjectExporter.h"
#include <codecvt>

using namespace std::chrono;

ZXCWriter::ZXCWriter(
	const std::wstring & ExporterVersion,
	const std::wstring & Filename,
	const SceneInfo & sceneInfo,
	const std::vector<ZXCMaterial>& material,
	std::vector<std::unique_ptr<ZXCObject>>& object,
	AnimationClip * clip)
	: mExporterVersion(ExporterVersion),
	mFilename(Filename),
	mSceneInfo(sceneInfo),
	mMaterial(material),
	mAnimation(clip)
{
	mObjects = std::move(object);
}

bool ZXCWriter::Savefile()
{
	struct Subset
	{
		UINT VertexStart;
		UINT VertexCount;
		UINT FaceStart;
		UINT FaceCount;
	};

	std::wofstream os;
	os.open(mFilename.c_str());

	if (!os.is_open()) return false;

	std::wstring nowTime = Savetime();

	UINT numMaterials;
	UINT numVertices = 0;
	UINT numTriangles = 0;
	UINT numHelpers = 0;
	UINT numMeshes = 0;
	UINT numAnimationClip = 0;
	if (mAnimation != nullptr)
		numAnimationClip = 1;

	numMaterials = (UINT)mMaterial.size();

	std::map<std::pair<UINT, int>, std::vector<std::pair<int, Subset>>> subsetID;
	
	for (auto& x : mObjects)
	{
		if (x->Type == ObjectType::MESH)
			++numMeshes;
		else
			++numHelpers;
		if (!x->mVertices.empty())
		{
			for (auto&k : x->mVertices)
			{
				Subset subset;
				subset.VertexCount = (UINT)k.second.size();
				subset.VertexStart = numVertices;
				subset.FaceCount = (UINT)x->mIndices[k.first].size();
				subset.FaceStart = numTriangles;
				numVertices += subset.VertexCount;
				numTriangles += subset.FaceCount;
				subsetID[std::pair<UINT, int>(x->mMaterialRef, k.first)].push_back(std::pair<int, Subset>((int)x->mNodeName.second, subset));
			}
		}
	}

	std::wstring info = 
		L"#Materials " + std::to_wstring(numMaterials) 
		+ L"\n#Helpers " + std::to_wstring(numHelpers) + 
		+ L"\n#Meshes " + std::to_wstring(numMeshes) +
		+ L"\n#Vertices " + std::to_wstring(numVertices) + 
		+ L"\n#Triangles " + std::to_wstring(numTriangles) + 
		+ L"\n#AnimationClips " + std::to_wstring(numAnimationClip);

	std::wstring header = L"**********ZXC_Header**********\n#" + mExporterVersion + L"\n#" + Savetime();
	os << header << info;

	WriteScene(os);
	WriteMaterial(os);
	WriteHelper(os);
	WriteMesh(os);

	std::wstring subsetHeader = L"\n**********SubsetTable**********";
	os << subsetHeader;
	for (auto & x : subsetID)
	{
		for (auto & k : x.second)
		{
			std::wstring subinfo = L"\nSubsetID: " + std::to_wstring(k.first) + L" " + std::to_wstring(x.first.first) + L" " + std::to_wstring(x.first.second) +
				L" VertexStart: " + std::to_wstring(k.second.VertexStart) + L" VertexCount: " + std::to_wstring(k.second.VertexCount) +
				L" FaceStart: " + std::to_wstring(k.second.FaceStart) + L" FaceCount: " + std::to_wstring(k.second.FaceCount);

			os << subinfo;
		}
	}

	os << std::endl;

	WriteVertex(os);
	if(numAnimationClip > 0)
		WriteAnimations(os);

	return true;
}

void ZXCWriter::WriteScene(std::wofstream & os)
{
	std::wstring sceneStart = L"\n\n**********Scene_Info**********";
	std::wstring firstFrame = L"\nFirstFrmae: " + std::to_wstring(mSceneInfo.FirstFrame);
	std::wstring lastFrame = L"\nLastFrame: " + std::to_wstring(mSceneInfo.LastFrame);
	std::wstring frameSpeed = L"\nFrameSpeed: " + std::to_wstring(mSceneInfo.FrameSpeed);
	std::wstring tickPerFrame = L"\nTickPerFrame: " + std::to_wstring(mSceneInfo.TickperFrame);

	os << sceneStart << firstFrame << lastFrame << frameSpeed << tickPerFrame;
}

void ZXCWriter::WriteMaterial(std::wofstream & os)
{
	std::wstring rootMtlInfo = L"\n\n**********Materials**********";

	os << rootMtlInfo;

	for (UINT i = 0; i < (UINT)mMaterial.size(); ++i)
	{
		std::wstring mtlInfo =
			L"\n#Name " + mMaterial[i].Name +
			L"\n#MapNum " + std::to_wstring(mMaterial[i].TexMap.size()) +
			L"\n#SubMtlNum " + std::to_wstring(mMaterial[i].SubMaterial.size());

		os << mtlInfo;

		if (!mMaterial[i].SubMaterial.empty())
		{
			for (int k = 0; k < mMaterial[i].SubMaterial.size(); ++k)
			{
				std::wstring subMtlInfo =
					L"\n\t#SubMtlName " + mMaterial[i].SubMaterial[k].Name +
					L"\n\t#MapNum " + std::to_wstring(mMaterial[i].SubMaterial[k].TexMap.size());

				os << subMtlInfo;

				if (!mMaterial[i].SubMaterial[k].TexMap.empty())
				{
					for (auto &x : mMaterial[i].SubMaterial[k].TexMap)
					{
						std::wstring TexInfo =
							L"\n\t#MapSubNO " + std::to_wstring(x.SubNo) +
							L"\n\t#TexFile " + x.Filename;
						os << TexInfo;
					}
				}
				os << std::endl;
			}
		}
		else
		{
			if (!mMaterial[i].TexMap.empty())
			{
				for (auto &x : mMaterial[i].TexMap)
				{
					std::wstring TexInfo =
						L"\n\t\t#MapSubNO" + std::to_wstring(x.SubNo) +
						L"\t#TexFile " + x.Filename;
					os << TexInfo;
				}
				os << std::endl;
			}
		}
	}
}

void ZXCWriter::WriteMesh(std::wofstream & os)
{
	std::wstring meshHead = L"\n**********Meshed**********";

	os << meshHead;

	for (auto &x : mObjects)
	{
		if (x->Type != ObjectType::MESH)
			continue;

		std::wstring meshInfo = L"\n#Name " + x->mNodeName.first + L" " + std::to_wstring(x->mNodeName.second) +
			L"\n#ParentName " + x->mParentName.first + L" " + std::to_wstring((int)x->mParentName.second) +
			L"\n#Node_TM\n\t#TM_ROW0 " + std::to_wstring(x->mWorld.m[0][0]) + L"  " + std::to_wstring(x->mWorld.m[0][1]) + L"  " + std::to_wstring(x->mWorld.m[0][2]) + L"  " + std::to_wstring(x->mWorld.m[0][3]) +
					  L"\n\t#TM_ROW1 " + std::to_wstring(x->mWorld.m[1][0]) + L"  " + std::to_wstring(x->mWorld.m[1][1]) + L"  " + std::to_wstring(x->mWorld.m[1][2]) + L"  " + std::to_wstring(x->mWorld.m[1][3]) +
					  L"\n\t#TM_ROW2 " + std::to_wstring(x->mWorld.m[2][0]) + L"  " + std::to_wstring(x->mWorld.m[2][1]) + L"  " + std::to_wstring(x->mWorld.m[2][2]) + L"  " + std::to_wstring(x->mWorld.m[2][3]) +
					  L"\n\t#TM_ROW3 " + std::to_wstring(x->mWorld.m[3][0]) + L"  " + std::to_wstring(x->mWorld.m[3][1]) + L"  " + std::to_wstring(x->mWorld.m[3][2]) + L"  " + std::to_wstring(x->mWorld.m[3][3]) +
					  L"\n\t#TM_POS " + std::to_wstring(x->InititalPos.t.x) + L" " + std::to_wstring(x->InititalPos.t.y) + L" " + L" " + std::to_wstring(x->InititalPos.t.z) +
					  L"\n\t#TM_ROTQUAT " + std::to_wstring(x->InititalPos.q.x) + L" " + std::to_wstring(x->InititalPos.q.y) + L" " + std::to_wstring(x->InititalPos.q.z) + L" " + std::to_wstring(x->InititalPos.q.w) +
					  L"\n\t#TM_SCALEQUAT " + std::to_wstring(x->InititalPos.u.x) + L" " + std::to_wstring(x->InititalPos.u.y) + L" " + std::to_wstring(x->InititalPos.u.z) + L" " + std::to_wstring(x->InititalPos.q.w) +
					  L"\n\t#TM_SCALE " + std::to_wstring(x->InititalPos.k.x) + L" " + std::to_wstring(x->InititalPos.k.y) + L" " + std::to_wstring(x->InititalPos.k.z);

		os << meshInfo << std::endl;
	}
}

void ZXCWriter::WriteHelper(std::wofstream & os)
{
	std::wstring helperHead = L"\n**********Helpers**********";

	os << helperHead;

	for (auto&x : mObjects)
	{
		if (x->Type == ObjectType::MESH)
			continue;
		
		std::wstring hclass;
		if (x->Type == ObjectType::BIPED)
		{
			hclass = L"Biped";
		}
		else if (x->Type == ObjectType::BONE)
		{
			hclass = L"Bone";
		}
		else if (x->Type == ObjectType::DUMMY)
		{
			hclass = L"Dummy";
		}

		std::wstring helperInfo = L"\n#Name " + x->mNodeName.first + L" " + std::to_wstring(x->mNodeName.second) +
			L"\n#ParentName " + x->mParentName.first + L" " + std::to_wstring((int)x->mParentName.second) +
			L"\n#Class " + hclass +
			L"\n#Node_TM\n\t#TM_ROW0 " + std::to_wstring(x->mWorld.m[0][0]) + L"  " + std::to_wstring(x->mWorld.m[0][1]) + L"  " + std::to_wstring(x->mWorld.m[0][2]) + L"  " + std::to_wstring(x->mWorld.m[0][3]) +
					  L"\n\t#TM_ROW1 " + std::to_wstring(x->mWorld.m[1][0]) + L"  " + std::to_wstring(x->mWorld.m[1][1]) + L"  " + std::to_wstring(x->mWorld.m[1][2]) + L"  " + std::to_wstring(x->mWorld.m[1][3]) +
					  L"\n\t#TM_ROW2 " + std::to_wstring(x->mWorld.m[2][0]) + L"  " + std::to_wstring(x->mWorld.m[2][1]) + L"  " + std::to_wstring(x->mWorld.m[2][2]) + L"  " + std::to_wstring(x->mWorld.m[2][3]) +
					  L"\n\t#TM_ROW3 " + std::to_wstring(x->mWorld.m[3][0]) + L"  " + std::to_wstring(x->mWorld.m[3][1]) + L"  " + std::to_wstring(x->mWorld.m[3][2]) + L"  " + std::to_wstring(x->mWorld.m[3][3]) +
					  L"\n\t#TM_POS " + std::to_wstring(x->InititalPos.t.x) + L" " + std::to_wstring(x->InititalPos.t.y) + L" " + L" " + std::to_wstring(x->InititalPos.t.z) +
					  L"\n\t#TM_ROTQUAT " + std::to_wstring(x->InititalPos.q.x) + L" " + std::to_wstring(x->InititalPos.q.y) + L" " + std::to_wstring(x->InititalPos.q.z) + L" " + std::to_wstring(x->InititalPos.q.w) +
					  L"\n\t#TM_SCALEQUAT " + std::to_wstring(x->InititalPos.u.x) + L" " + std::to_wstring(x->InititalPos.u.y) + L" " + std::to_wstring(x->InititalPos.u.z) + L" " + std::to_wstring(x->InititalPos.q.w) +
					  L"\n\t#TM_SCALE " + std::to_wstring(x->InititalPos.k.x) + L" " + std::to_wstring(x->InititalPos.k.y) + L" " + std::to_wstring(x->InititalPos.k.z) +
					  L"\n#BoundingBoxMin " + std::to_wstring(x->mBoundingBox.pmin.x) + L" " + std::to_wstring(x->mBoundingBox.pmin.y) + L" " + std::to_wstring(x->mBoundingBox.pmin.z) +
					  L"\n#BoundingBoxMax " + std::to_wstring(x->mBoundingBox.pmax.x) + L" " + std::to_wstring(x->mBoundingBox.pmax.y) + L" " + std::to_wstring(x->mBoundingBox.pmax.z);

		os << helperInfo << std::endl;
	}
}

void ZXCWriter::WriteVertex(std::wofstream & os)
{
	std::wstring vertexHeader = L"\n**********Vertices**********";

	os << vertexHeader;

	for (auto&x : mObjects)
	{
		for (auto&k : x->mVertices)
		{
			for (auto& m : k.second)
			{
				std::wstring vertexInfo = L"\nPosition: " + std::to_wstring(m.p.x) + L" " + std::to_wstring(m.p.y) + L" " + std::to_wstring(m.p.z) +
					L"\nNormal: " + std::to_wstring(m.n.x) + L" " + std::to_wstring(m.n.y) + L" " + std::to_wstring(m.n.z) +
					L"\nColor: " + std::to_wstring(m.c.x) + L" " + std::to_wstring(m.c.y) + L" " + std::to_wstring(m.c.z) + L" " + std::to_wstring(m.c.w) +
					L"\nTex-Coords: " + std::to_wstring(m.t.x) + L" " + std::to_wstring(m.t.y);

				os << vertexInfo << std::endl;
			}
		}
	}

	std::wstring indexHeader = L"\n**********Index**********\n";

	os << indexHeader;

	for (auto&x : mObjects)
	{
		for (auto&k : x->mIndices)
		{
			for (UINT i = 0; i < k.second.size() / 3; ++i)
			{
				std::uint32_t i0 = k.second[i * 3 + 0];
				std::uint32_t i1 = k.second[i * 3 + 1];
				std::uint32_t i2 = k.second[i * 3 + 2];

				std::wstring indexInfo = std::to_wstring(i0) + L" " + std::to_wstring(i1) + L" " + std::to_wstring(i2);
				os << indexInfo << std::endl;
			}
		}
	}

	os << std::endl;
}

void ZXCWriter::WriteAnimations(std::wofstream & os)
{
	std::wstring aniHeader = L"\n**********AnimationClip**********";
	for (UINT i=0; i < (UINT)mAnimation->BoneAnimations.size(); ++i)
	{
		auto& ani = mAnimation->BoneAnimations[i].Keyframes;
		std::wstring aniInfo = L"\nNodeIndex: " + std::to_wstring(i) + L" FrameNum: " + std::to_wstring(ani.size());
		os << aniInfo;
		for (UINT j = 0; j < (UINT)mAnimation->BoneAnimations[i].Keyframes.size(); ++j)
		{
			std::wstring trackInfo = 
				L"\n\tTick: " + std::to_wstring(ani[j].Tick) +
				L" Translation: " + std::to_wstring(ani[j].Translations.x) + L" " + std::to_wstring(ani[j].Translations.y) + L" " + std::to_wstring(ani[j].Translations.z) +
				L" Scale: " + std::to_wstring(ani[j].Scale.x) + L" " + std::to_wstring(ani[j].Scale.y) + L" " + std::to_wstring(ani[j].Scale.z) +
				L" ScaleQuat: " + std::to_wstring(ani[j].ScaleQuat.x) + L" " + std::to_wstring(ani[j].ScaleQuat.y) + L" " + std::to_wstring(ani[j].ScaleQuat.z) + L" " + std::to_wstring(ani[j].ScaleQuat.w) +
				L" RotateQuat: " + std::to_wstring(ani[j].RotationQuat.x) + L" " + std::to_wstring(ani[j].RotationQuat.y) + L" " + std::to_wstring(ani[j].RotationQuat.z) + L" " + std::to_wstring(ani[j].RotationQuat.w);

			os << trackInfo;
		}
		os << std::endl;
	}
}


std::wstring ZXCWriter::Savetime()
{
	system_clock::time_point now = system_clock::now();
	time_t nowTime = system_clock::to_time_t(now);

	char buf[256] = { 0, };
	wchar_t wbuf[256] = { 0, };
	ctime_s(buf, sizeof(buf), &nowTime);

	MultiByteToWideChar(CP_ACP, 0, buf, -1, wbuf, 256);

	return std::wstring(wbuf);
}