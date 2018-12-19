
#include "ZXCWriter.h"
#include <codecvt>

using namespace std::chrono;

ZXCWriter::ZXCWriter(
	const std::wstring & ExporterVersion,
	const std::wstring & Filename,
	const SceneInfo & sceneInfo,
	const std::vector<ZXCMaterial>& material,
	std::vector<ZXCObject>& mesh,
	std::vector<ZXCObject>& helper)
	: mExporterVersion(ExporterVersion),
	mFilename(Filename),
	mSceneInfo(sceneInfo),
	mMaterial(material)
{
	mMesh = std::move(mesh);
	mHelper = std::move(helper);
}

bool ZXCWriter::Savefile()
{
	std::wofstream os;
	os.open(mFilename.c_str());

	if (!os.is_open()) return false;

	std::wstring nowTime = Savetime();

	os << mExporterVersion << std::endl << nowTime << std::endl;

	InputScene(os);
	InputMaterial(os);
	InputMesh(os);
	InputHelper(os);

	return true;
}

void ZXCWriter::InputScene(std::wofstream & os)
{
	std::wstring sceneStart = L"#SCENEINFO";
	std::wstring firstFrame = L"\n\tFirstFrmae: " + std::to_wstring(mSceneInfo.FirstFrame);
	std::wstring lastFrame = L"\n\tLastFrame: " + std::to_wstring(mSceneInfo.LastFrame);
	std::wstring frameSpeed = L"\n\tFrameSpeed: " + std::to_wstring(mSceneInfo.FrameSpeed);
	std::wstring tickPerFrame = L"\n\tTickPerFrame: " + std::to_wstring(mSceneInfo.TickperFrame) + L"\n";

	os << sceneStart << firstFrame << lastFrame << frameSpeed << tickPerFrame;
}

void ZXCWriter::InputMaterial(std::wofstream & os)
{
	std::wstring rootMtlInfo = L"\n#MATERIAL_NUM " + std::to_wstring(mMaterial.size());

	os << rootMtlInfo;

	for (int i = 0; i < (int)mMaterial.size(); ++i)
	{
		std::wstring mtlInfo = 
			L"\n\t#MATERIAL_INDEX " + std::to_wstring(i) +
			L"\t#MATERIAL_NAME " + mMaterial[i].Name +
			L"\t#MAP_NUM " + std::to_wstring(mMaterial[i].TexMap.size()) + 
			L"\t#SUBMTL_NUM " + std::to_wstring(mMaterial[i].SubMaterial.size());

		os << mtlInfo;

		if (!mMaterial[i].SubMaterial.empty())
		{
			for (int k = 0; k < mMaterial[i].SubMaterial.size(); ++k)
			{
				std::wstring subMtlInfo = 
					L"\n\t\t#SUBMTL_INDEX " + std::to_wstring(k) +
					L"\t#MATERIAL_NAME " + mMaterial[i].SubMaterial[k].Name +
					L"\t#MAP_NUM " + std::to_wstring(mMaterial[i].SubMaterial[k].TexMap.size());

				os << subMtlInfo;

				if (!mMaterial[i].SubMaterial[k].TexMap.empty())
				{
					for (auto &x : mMaterial[i].SubMaterial[k].TexMap)
					{
						std::wstring TexInfo = 
							L"\n\t\t\t#MAP_SUBNO " + std::to_wstring(x.SubNo) +
							L"\t#TEX_FILE " + x.Filename;
						os << TexInfo;
					}
				}
			}
		}
		else
		{
			if (!mMaterial[i].TexMap.empty())
			{
				for (auto &x : mMaterial[i].TexMap)
				{
					std::wstring TexInfo = 
						L"\n\t\t#MAP_SUBNO " + std::to_wstring(x.SubNo) +
						L"\t#TEX_FILE " + x.Filename;
					os << TexInfo;
				}
			}
		}
	}
}

void ZXCWriter::InputMesh(std::wofstream & os)
{
	std::wstring MeshNumber = L"\n#GEOMMESH_NUM " + std::to_wstring(mMesh.size());
	os << MeshNumber;

	for (int i = 0; i < (int)mMesh.size(); ++i)
	{
		std::wstring MeshInfo =
			L"\n\t#GEOMESH_INDEX " + std::to_wstring(i) +
			L" \t#NODE_NAME " + mMesh[i].mNodeName +
			L" \t#NODE_PARENT " + mMesh[i].mParentName +
			L" \t#MATERIAL_REF " + std::to_wstring(mMesh[i].mMaterialRef) +
			L" \t#POS_ANIMATION " + std::to_wstring(mMesh[i].mPosTrack.size()) +
			L" \t#ROT_ANIMATION " + std::to_wstring(mMesh[i].mRotTrack.size()) +
			L" \t#SCALE_ANIMATION " + std::to_wstring(mMesh[i].mScaleTrack.size());

		std::wstring world =
			L"\n\t\t" + std::to_wstring(mMesh[i].mWorld.m[0][0]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[0][1]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[0][2]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[0][3]) +
			L"\n\t\t" + std::to_wstring(mMesh[i].mWorld.m[1][0]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[1][1]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[1][2]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[1][3]) +
			L"\n\t\t" + std::to_wstring(mMesh[i].mWorld.m[2][0]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[2][1]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[2][2]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[2][3]) +
			L"\n\t\t" + std::to_wstring(mMesh[i].mWorld.m[3][0]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[3][1]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[3][2]) + L" \t" + std::to_wstring(mMesh[i].mWorld.m[3][3]);

		std::wstring worldMatrix =
			L"\n\t\t#WORLD_MATRIX" + world;

		os << MeshInfo << worldMatrix;

		std::sort(mMesh[i].mTriangles.begin(), mMesh[i].mTriangles.end());

		int mtrlRef = mMesh[i].mMaterialRef;
		if (mtrlRef >= 0 && !mMaterial[mtrlRef].SubMaterial.empty())
		{
			for (int j = 0; j < mMaterial[mtrlRef].SubMaterial.size(); ++j)
			{
				InputVBIB(mMesh[i], j, os);
			}
		}
		else
		{
			InputVBIB(mMesh[i], -1, os);
		}
		
		if (!mMesh[i].mPosTrack.empty() ||
			!mMesh[i].mRotTrack.empty() ||
			!mMesh[i].mScaleTrack.empty())
		{
			InputAnimation(mMesh[i], os);
		}
	}
}

void ZXCWriter::InputHelper(std::wofstream & os)
{
	std::wstring helperNumber = L"\n#HELPER_NUM " + std::to_wstring(mHelper.size());
	os << helperNumber;

	for (int i = 0; i < (int)mHelper.size(); ++i)
	{
		std::wstring helperInfo =
			L"\n\t#HELPER_INDEX " + std::to_wstring(i) +
			L" \t#NODE_NAME " + mHelper[i].mNodeName +
			L" \t#NODE_PARENT " + mHelper[i].mParentName +
			L" \t#POS_ANIMATION " + std::to_wstring(mHelper[i].mPosTrack.size()) +
			L" \t#ROT_ANIMATION " + std::to_wstring(mHelper[i].mRotTrack.size()) +
			L" \t#SCALE_ANIMATION " + std::to_wstring(mHelper[i].mScaleTrack.size());

		std::wstring world =
			L"\n\t\t" + std::to_wstring(mHelper[i].mWorld.m[0][0]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[0][1]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[0][2]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[0][3]) +
			L"\n\t\t" + std::to_wstring(mHelper[i].mWorld.m[1][0]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[1][1]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[1][2]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[1][3]) +
			L"\n\t\t" + std::to_wstring(mHelper[i].mWorld.m[2][0]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[2][1]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[2][2]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[2][3]) +
			L"\n\t\t" + std::to_wstring(mHelper[i].mWorld.m[3][0]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[3][1]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[3][2]) + L" \t" + std::to_wstring(mHelper[i].mWorld.m[3][3]);

		std::wstring worldMatrix =
			L"\n\t\t#WORLD_MATRIX" + world;

		os << helperInfo << worldMatrix;

		if (!mHelper[i].mPosTrack.empty() ||
			!mHelper[i].mRotTrack.empty() ||
			!mHelper[i].mScaleTrack.empty())
		{
			InputAnimation(mHelper[i], os);
		}
	}
}

void ZXCWriter::InputVBIB(ZXCObject& mesh, int mtrlRef, std::wofstream & os)
{
	std::vector<PNCT_VERTEX> vertices;
	std::vector<std::uint32_t> indices;
	for (int i = 0; i < (int)mesh.mTriangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			if (mtrlRef != -1 && mesh.mTriangles[i].mSubMtrl != mtrlRef) break;
			int vNumber = [&vertices](PNCT_VERTEX& rhs) -> int
			{
				for (int k = 0; k < (int)vertices.size(); ++k)
				{
					if (vertices[k] == rhs)
					{
						return k;
					}
				}
				return -1;
			}(mesh.mTriangles[i].v[j]);
			if (vNumber == -1)
			{
				vertices.push_back(mesh.mTriangles[i].v[j]);
				indices.push_back((UINT)(vertices.size() - 1));
			}
			else
			{
				indices.push_back(vNumber);
			}
		}
	}

	for (int i = 0; i < (int)indices.size() / 3; ++i)
	{
		std::uint32_t i0 = indices[i * 3 + 0];
		std::uint32_t i1 = indices[i * 3 + 1];
		std::uint32_t i2 = indices[i * 3 + 2];

		std::set<std::uint32_t> index;
		index.insert(i0);
		index.insert(i1);
		index.insert(i2);

		indices[i * 3 + 0] = *index.begin();
		indices[i * 3 + 1] = *std::next(index.begin(), 1);
		indices[i * 3 + 2] = *std::next(index.begin(), 2);

		for (int k = i; k >= 1; --k)
		{
			std::uint32_t n0 = indices[k * 3 + 0];
			std::uint32_t n1 = indices[k * 3 + 1];
			std::uint32_t n2 = indices[k * 3 + 2];

			std::uint32_t p0 = indices[(k - 1) * 3 + 0];
			std::uint32_t p1 = indices[(k - 1) * 3 + 1];
			std::uint32_t p2 = indices[(k - 1) * 3 + 2];

			if (n0 < p0)
			{
				std::swap(indices[(k - 1) * 3 + 0], indices[k * 3 + 0]);
				std::swap(indices[(k - 1) * 3 + 1], indices[k * 3 + 1]);
				std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
				continue;
			}

			if (n0 == p0 && n1 < p1)
			{
				std::swap(indices[(k - 1) * 3 + 1], indices[k * 3 + 1]);
				std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
				continue;
			}

			if (n0 == p0 && n1 == p1 && n2 < p2)
			{
				std::swap(indices[(k - 1) * 3 + 2], indices[k * 3 + 2]);
				continue;
			}
			break;
		}
	}

	if (vertices.empty())
		return;

	std::wstring vInfo = L"\n\t#MESH_MTLID " + std::to_wstring(mtrlRef) +
		L"\n\t#VERTEX_NUMBER " + std::to_wstring(vertices.size());

	os << vInfo;

	for (int i = 0; i < (int)vertices.size(); ++i)
	{
		auto v = vertices[i];
		std::wstring Vertex = VertexToString(v);
		os << Vertex;
	}

	std::wstring iInfo = L" \n\t#FACE_NUMBER " + std::to_wstring(indices.size() / 3);

	os << iInfo;

	for (int i = 0; i < (int)indices.size() / 3; ++i)
	{
		std::uint32_t i0 = indices[i * 3 + 0];
		std::uint32_t i1 = indices[i * 3 + 1];
		std::uint32_t i2 = indices[i * 3 + 2];

		std::wstring Index = L"\n\t\t" + std::to_wstring(i0) + L" " + std::to_wstring(i1) + L" " + std::to_wstring(i2);

		os << Index;
	}
}

void ZXCWriter::InputAnimation(const ZXCObject & obj, std::wofstream & os)
{
	std::wstring ani = L"\n\t\t#TM_ANIMATION";

	os << ani;

	if (!obj.mPosTrack.empty())
	{
		std::wstring posAni = L"\n\t\t\t#CONTROL_POS_TRACK";
		os << posAni;

		for (int i = 0; i < (int)obj.mPosTrack.size(); ++i)
		{
			auto o = obj.mPosTrack[i];
			std::wstring posInfo = L"\n\t\t\t\t#CONTROL_POS_SAMPLE " + std::to_wstring(o.mTick) +
				L"\t" + std::to_wstring(o.mVec.x) +
				L"\t" + std::to_wstring(o.mVec.y) +
				L"\t" + std::to_wstring(o.mVec.z);

			os << posInfo;
		}
	}

	if (!obj.mRotTrack.empty())
	{
		std::wstring rotAni = L"\n\t\t\t#CONTROL_ROT_TRACK";
		os << rotAni;

		for (int i = 0; i < (int)obj.mRotTrack.size(); ++i)
		{
			auto o = obj.mRotTrack[i];
			std::wstring rotInfo = L"\n\t\t\t\t#CONTROL_ROT_SAMPLE " + std::to_wstring(o.mTick) +
				L"\t" + std::to_wstring(o.mQuat.x) +
				L"\t" + std::to_wstring(o.mQuat.y) +
				L"\t" + std::to_wstring(o.mQuat.z) +
				L"\t" + std::to_wstring(o.mQuat.w);

			os << rotInfo;
		}
	}

	if (!obj.mScaleTrack.empty())
	{
		std::wstring scaleAni = L"\n\t\t\t#CONTROL_SCALE_TRACK";
		os << scaleAni;

		for (int i = 0; i < (int)obj.mScaleTrack.size(); ++i)
		{
			auto o = obj.mScaleTrack[i];
			std::wstring scaleInfo = L"\n\t\t\t\t#CONTROL_SCALE_SAMPLE " + std::to_wstring(o.mTick) +
				L"\t" + std::to_wstring(o.mQuat.x) +
				L"\t" + std::to_wstring(o.mQuat.y) +
				L"\t" + std::to_wstring(o.mQuat.z) +
				L"\t" + std::to_wstring(o.mQuat.w) +
				L"\t" + std::to_wstring(o.mVec.x) +
				L"\t" + std::to_wstring(o.mVec.y) +
				L"\t" + std::to_wstring(o.mVec.z);

			os << scaleInfo;
		}
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

std::wstring ZXCWriter::VertexToString(const PNCT_VERTEX & v)
{
	std::wstring buf = L"\n\t\t#POSITION " + std::to_wstring(v.p.x) + L" " + std::to_wstring(v.p.y) + L" " + std::to_wstring(v.p.z) + L" " +
		L"\t#NORMAL " + std::to_wstring(v.n.x) + L" " + std::to_wstring(v.n.y) + L" " + std::to_wstring(v.n.z) + L" " +
		L"\t#COLOR " + std::to_wstring(v.c.x) + L" " + std::to_wstring(v.c.y) + L" " + std::to_wstring(v.c.z) + L" " + std::to_wstring(v.c.w) + L" " +
		L"\t#TEXCOORD " + std::to_wstring(v.t.x) + L" " + std::to_wstring(v.t.y);

	return buf;
}
