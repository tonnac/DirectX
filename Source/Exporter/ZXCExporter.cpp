#include "ZXCExporter.h"

ZXCExporter::ZXCExporter()
{
	std::locale::global(std::locale("Korean"));
}

bool ZXCExporter::Initialize(Interface* maxinterface, const MCHAR* Filename, const MCHAR* Longdesc)
{
	mMaxInterface = maxinterface;
	mRootNode = mMaxInterface->GetRootNode();
	mVersion = Longdesc;

	if (mRootNode == nullptr && maxinterface == nullptr) return false;

	mFilename = Filename;
	mFilename.assign(mFilename, mFilename.find_last_of('\\') + 1, mFilename.length());
	mInterval = mMaxInterface->GetAnimRange();

	mSceneInfo.FrameSpeed = GetFrameRate();
	mSceneInfo.TickperFrame = GetTicksPerFrame();
	mSceneInfo.FirstFrame = mInterval.Start() / mSceneInfo.TickperFrame;
	mSceneInfo.LastFrame = mInterval.End() / mSceneInfo.TickperFrame;

	LoadNode(mRootNode);
	LoadMaterial();
	
	if (!mWriter.Savefile(mVersion, mFilename, mSceneInfo, mOutputMaterial)) return false;

	return true;
}

void ZXCExporter::Run()
{
}

void ZXCExporter::LoadNode(INode * node)
{
	if (node == nullptr) return;

	AddObject(node);
	AddMaterial(node);

	int ChildNum = node->NumberOfChildren();
	for (int i = 0; i < ChildNum; ++i)
	{
		INode* childNode = node->GetChildNode(i);
		LoadNode(childNode);
	}
}

void ZXCExporter::AddObject(INode * node)
{
	ObjectState os = node->EvalWorldState(mInterval.Start());

	if (os.obj == nullptr || os.obj->ClassID() == Class_ID(TARGET_CLASS_ID, 0))
	{
		return;
	}

	switch (os.obj->SuperClassID())
	{
		case GEOMOBJECT_CLASS_ID:
		{
			// ObjectÃß°¡
		}break;
		case HELPER_CLASS_ID:
		{

		}break;
	}

}

void ZXCExporter::AddMaterial(INode * node)
{
	Mtl* mtl = node->GetMtl();
	if (mtl == nullptr)
	{
		return;
	}

	for (int i = 0; i < mMaxMaterial.size(); ++i)
	{
		if (mMaxMaterial[i] == mtl)
		{
			return;
		}
	}

	mMaxMaterial.push_back(mtl);
}

void ZXCExporter::LoadMaterial()
{
	for (int i = 0; i < (int)mMaxMaterial.size(); ++i)
	{
		Mtl* mtl = mMaxMaterial[i];

		ZXCMaterial outMtl;
		TSTR className;

		mtl->GetClassName(className);
		outMtl.Name = mtl->GetName();
		std::ReplaceString(outMtl.Name);
		outMtl.ClassName = className;
		outMtl.SubMaterial.resize(mtl->NumSubMtls());

		if (!outMtl.SubMaterial.empty())
		{
			for (int k = 0; k < (int)outMtl.SubMaterial.size(); ++k)
			{
				Mtl* subMtl = mtl->GetSubMtl(k);

				ZXCMaterial outSubMtl;

				TSTR subClassname;
				outSubMtl.Name = subMtl->GetName();
				std::ReplaceString(outSubMtl.Name);
				subMtl->GetClassName(subClassname);
				outSubMtl.ClassName = subClassname;

				if (subMtl != nullptr)
				{
					LoadTexture(outSubMtl, subMtl);
				}
				outMtl.SubMaterial[k] = outSubMtl;
			}
		}
		else
		{
			LoadTexture(outMtl, mtl);
		}
		mOutputMaterial.push_back(outMtl);
	}
}

void ZXCExporter::LoadTexture(ZXCMaterial & zxcMtl, Mtl * srcMtl)
{
	for (int i = 0; i < srcMtl->NumSubTexmaps(); ++i)
	{
		Texmap* tex = srcMtl->GetSubTexmap(i);
		if (tex == nullptr)	continue;

		if (srcMtl->ClassID() == Class_ID(DMTL_CLASS_ID, 0))
		{
			StdMat * std = (StdMat*)srcMtl;
			if (!std->MapEnabled(i)) continue;
		}

		ZXCTexmap texMap;
		texMap.SubNo = i;

		if (tex->ClassID() == Class_ID(BMTEX_CLASS_ID, 0x00))
		{
			TSTR mapName = ((BitmapTex *)tex)->GetMapName();
			TSTR fullName;
			SplitPathFile(mapName, &fullName, &mapName);
			texMap.Filename = mapName;
		}
		zxcMtl.TexMap.push_back(texMap);
	}
}
