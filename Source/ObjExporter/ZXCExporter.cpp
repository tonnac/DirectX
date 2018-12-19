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

	return true;
}

bool ZXCExporter::Run()
{
	LoadScene();
	LoadNode(mRootNode);
	LoadMaterial();
	LoadObject(mGeomObjects);
	LoadObject(mHelperObjects);

	mWriter = std::make_unique<ZXCWriter>(mVersion, mFilename, mSceneInfo, mOutputMaterial, mOutputGeomObjects, mOutputHelperObjects);
	if (!mWriter->Savefile()) return false;

	return true;
}

void ZXCExporter::LoadScene()
{
	mSceneInfo.FrameSpeed = GetFrameRate();
	mSceneInfo.TickperFrame = GetTicksPerFrame();
	mSceneInfo.FirstFrame = mInterval.Start() / mSceneInfo.TickperFrame;
	mSceneInfo.LastFrame = mInterval.End() / mSceneInfo.TickperFrame;
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

void ZXCExporter::LoadAnimation(INode * node, ZXCObject & o)
{
	bool isPosAnimation;
	bool isRotAnimation;
	bool isScaleAnimation;

	if (CheckForAnimation(node, isPosAnimation, isRotAnimation, isScaleAnimation))
	{
		if (isPosAnimation)
		{
			InputPosSample(node, o);
		}

		if (isRotAnimation)
		{
			InputRotSample(node, o);
		}

		if (isScaleAnimation)
		{
			InputSclaeSample(node, o);
		}

		OverlappedTrackErase(o);
	}
}

void ZXCExporter::LoadObject(const std::vector<INode*> objects)
{
	for (auto& x : objects)
	{
		ZXCObject maxObj;
		maxObj.mNodeName = x->GetName();
		INode* parent = x->GetParentNode();

		maxObj.mParentName = L"NONE";
		if (parent != nullptr && (!parent->IsRootNode()))
		{
			maxObj.mParentName = parent->GetName();
			std::ReplaceString(maxObj.mParentName);
		}

		std::ReplaceString(maxObj.mNodeName);

		Matrix3 world = x->GetNodeTM(mInterval.Start());
		ConvertMatrix(world, maxObj.mWorld);

		LoadMesh(x, maxObj);
		LoadAnimation(x, maxObj);

		ObjectState os = x->EvalWorldState(mInterval.Start());
		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			mOutputGeomObjects.push_back(maxObj);
			break;
		case HELPER_CLASS_ID:
			mOutputHelperObjects.push_back(maxObj);
			break;
		default:
			break;
		}
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
			mGeomObjects.push_back(node);
		}break;
		case HELPER_CLASS_ID:
		{
			if (node->NumChildren() == 0)
			{
				return;
			}
			mHelperObjects.push_back(node);
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

void ZXCExporter::LoadMesh(INode* node, ZXCObject& o)
{
	Matrix3 tm = node->GetObjTMAfterWSM(mInterval.Start());

	ObjectState os = node->EvalWorldState(mInterval.Start());
	if (os.obj == nullptr || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
		return;

	bool needDel;
	TriObject * tri = GetTriObject(os.obj, mInterval.Start(), needDel);
	if (tri == nullptr) return;

	Mesh mesh = tri->GetMesh();
	mesh.buildBoundingBox();
	Box3 box = mesh.getBoundingBox(&tm);
	ConvertVector(box.pmin, o.mBoundingBox.pmin);
	ConvertVector(box.pmax, o.mBoundingBox.pmax);

	std::uint32_t i0, i1, i2;
	i0 = 0, i1 = 1, i2 = 2;
	if (isTMNegative(tm))
	{
		i0 = 2, i1 = 1, i2 = 0;
	}

	o.mMaterialRef = GetMaterialRef(node->GetMtl());
	o.mTriangles.resize(mesh.getNumFaces());
	for (int i = 0; i < (int)o.mTriangles.size(); ++i)
	{
		Matrix3 Inv = Inverse(node->GetNodeTM(mInterval.Start()));

		Point3 v;

		if (mesh.getNumVerts() > 0)
		{
			v = mesh.verts[mesh.faces[i].v[i0]] * tm * Inv;
			ConvertVector(v, o.mTriangles[i].v[0].p);

			v = mesh.verts[mesh.faces[i].v[i2]] * tm * Inv;
			ConvertVector(v, o.mTriangles[i].v[1].p);

			v = mesh.verts[mesh.faces[i].v[i1]] * tm * Inv;
			ConvertVector(v, o.mTriangles[i].v[2].p);
		}
		if (mesh.getNumTVerts() > 0)
		{
			o.mTriangles[i].v[0].t.x = mesh.tVerts[mesh.tvFace[i].t[i0]].x;
			o.mTriangles[i].v[0].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i0]].y;

			o.mTriangles[i].v[1].t.x = mesh.tVerts[mesh.tvFace[i].t[i2]].x;
			o.mTriangles[i].v[1].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i2]].y;

			o.mTriangles[i].v[2].t.x = mesh.tVerts[mesh.tvFace[i].t[i1]].x;
			o.mTriangles[i].v[2].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i1]].y;
		}
		if (mesh.getNumVertCol() > 0)
		{
			CopyMemory(&o.mTriangles[i].v[0].c, &mesh.vertCol[mesh.vcFace[i].t[i0]], sizeof(VertColor));
			CopyMemory(&o.mTriangles[i].v[1].c, &mesh.vertCol[mesh.vcFace[i].t[i2]], sizeof(VertColor));
			CopyMemory(&o.mTriangles[i].v[2].c, &mesh.vertCol[mesh.vcFace[i].t[i1]], sizeof(VertColor));
		}

		mesh.buildNormals();

		int vert = mesh.faces[i].getVert(i0);
		Point3 vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		ConvertVector(vn, o.mTriangles[i].v[0].n);
		
		vert = mesh.faces[i].getVert(i2);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		ConvertVector(vn, o.mTriangles[i].v[1].n);

		vert = mesh.faces[i].getVert(i1);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		ConvertVector(vn, o.mTriangles[i].v[2].n);

		o.mTriangles[i].mSubMtrl = mesh.faces[i].getMatID();
	}
	if (needDel)
		delete tri;
}

void ZXCExporter::ConvertMatrix(const Matrix3& maxMatrix,  D3D_MATRIX & dxMatrix)
{
	Point3 row0, row1, row2, row3;
	row0 = maxMatrix.GetRow(0);
	row1 = maxMatrix.GetRow(1);
	row2 = maxMatrix.GetRow(2);
	row3 = maxMatrix.GetRow(3);

	dxMatrix.m[0][0] = row0.x, dxMatrix.m[0][1] = row0.z, dxMatrix.m[0][2] = row0.y;
	dxMatrix.m[1][0] = row2.x, dxMatrix.m[1][1] = row2.z, dxMatrix.m[1][2] = row2.y;
	dxMatrix.m[2][0] = row1.x, dxMatrix.m[2][1] = row1.z, dxMatrix.m[2][2] = row1.y;
	dxMatrix.m[3][0] = row3.x, dxMatrix.m[3][1] = row3.z, dxMatrix.m[3][2] = row3.y;
}

TriObject * ZXCExporter::GetTriObject(Object* obj, TimeValue t, bool & isDelete)
{
	isDelete = false;
	if (obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
	{
		TriObject * tri = (TriObject*)obj->ConvertToType(t, Class_ID(TRIOBJ_CLASS_ID, 0));
		if (obj != tri) isDelete = true;

		return tri;
	}
	else
	{
		return nullptr;
	}
}

Point3 ZXCExporter::GetVertexNormal(Mesh & mesh, int faceNo, const RVertex & rv)
{
	Face* f = &mesh.faces[faceNo];
	DWORD smGroup = f->smGroup;
	int numNormals = rv.rFlags & NORCT_MASK;
	Point3 n;

	if (rv.rFlags & SPECIFIED_NORMAL)
	{
		n = rv.rn.getNormal();
	}
	else if (numNormals && smGroup)
	{
		if (numNormals == 1)
		{
			n = rv.rn.getNormal();
		}
		else
		{
			for (int i = 0; i < numNormals; ++i)
			{
				if (rv.ern[i].getSmGroup() & smGroup)
					n = rv.ern[i].getNormal();
			}
		}
	}
	else
	{
		n = mesh.getFaceNormal(faceNo);
	}
	return n;
}

int ZXCExporter::GetMaterialRef(Mtl * mtl)
{
	for (int i = 0; i < (int)mMaxMaterial.size(); ++i)
	{
		if (mMaxMaterial[i] == mtl)
		{
			return i;
		}
	}
	return -1;
}

bool ZXCExporter::isTMNegative(const Matrix3 & m)
{
	return (DotProd(CrossProd(m.GetRow(0), m.GetRow(1)), m.GetRow(2)) < 0.0f) ? true : false;
}

bool ZXCExporter::CheckForAnimation(INode * node, bool & isPos, bool & isRot, bool & isScale)
{
	AffineParts ap;
	Point3 firstPos, firstRotAxis, firstScaleFactor;
	float firstRotAngle;
	DecompAffine(mInterval.Start(), node, ap, &firstRotAxis, &firstRotAngle);
	firstPos = ap.t;
	firstScaleFactor = ap.k;


	TimeValue start = mInterval.Start() + mSceneInfo.TickperFrame;
	TimeValue end = mInterval.End();

	isPos = isRot = isScale = false;
	float rotAngle;
	Point3 rotAxis;

	for (TimeValue t = start; t <= end; t += mSceneInfo.TickperFrame)
	{
		DecompAffine(t, node, ap, &rotAxis, &rotAngle);

		if (!isPos)
		{
			if (!ap.t.Equals(firstPos, Epsilon))
			{
				isPos = true;
			}
		}

		if (!isRot)
		{
			if (fabsf(rotAngle - firstRotAngle) > Epsilon)
			{
				isRot = true;
			}
			else if(!firstRotAxis.Equals(rotAxis, Epsilon))
			{
				isRot = true;
			}
		}

		if (!isScale)
		{
			if (!ap.k.Equals(firstScaleFactor, Epsilon))
			{
				isScale = true;
			}
		}

		if (isPos && isRot && isScale) break;
	}
	return isPos || isRot || isScale;
}

void ZXCExporter::DecompAffine(TimeValue t, INode * node, AffineParts & ap, Point3 * rotAxis, float * rotAngle)
{
	Matrix3 tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
	decomp_affine(tm, &ap);

	if(rotAngle != nullptr)
		AngAxisFromQ(ap.q, rotAngle, *rotAxis);
}

void ZXCExporter::InputPosSample(INode * node, ZXCObject & o)
{
	TimeValue s = mInterval.Start();
	TimeValue e = mInterval.End();
	AffineParts ap;

	for (TimeValue t = s; t <= e; t += mSceneInfo.TickperFrame)
	{
		DecompAffine(t, node, ap);

		AnimTrack ani;
		ani.mTick = t;
		ConvertVector(ap.t, ani.mVec);
		o.mPosTrack.push_back(ani);
	}
}

void ZXCExporter::InputRotSample(INode * node, ZXCObject & o)
{
	TimeValue s = mInterval.Start();
	TimeValue e = mInterval.End();
	AffineParts ap;

	for (TimeValue t = s; t <= e; t += mSceneInfo.TickperFrame)
	{
		DecompAffine(t, node, ap);

		AnimTrack ani;
		ani.mTick = t;
		ConvertVector(ap.q, ani.mQuat);
		o.mRotTrack.push_back(ani);
	}
}

void ZXCExporter::InputSclaeSample(INode * node, ZXCObject & o)
{
	TimeValue s = mInterval.Start();
	TimeValue e = mInterval.End();
	AffineParts ap;

	for (TimeValue t = s; t <= e; t += mSceneInfo.TickperFrame)
	{
		DecompAffine(t, node, ap);

		AnimTrack ani;
		ani.mTick = t;
		ConvertVector(ap.u, ani.mQuat);
		ConvertVector(ap.k, ani.mVec);
		o.mScaleTrack.push_back(ani);
	}
}

void ZXCExporter::OverlappedTrackErase(ZXCObject & o)
{
	if (!o.mPosTrack.empty())
	{
		std::vector<AnimTrack>::iterator iter = o.mPosTrack.begin() + 1;
		for (;iter != o.mPosTrack.end() - 1;)
		{
			if (iter->mVec.Equals((iter - 1)->mVec, Epsilon) &&
				iter->mVec.Equals((iter + 1)->mVec, Epsilon))
			{
				iter = o.mPosTrack.erase(iter);
			}
			else
				++iter;
		}

		iter = o.mPosTrack.end() - 1;

		Quat r0 = (iter - 1)->mQuat;
		Quat r1 = iter->mQuat;

		if (r0.Equals(r1, Epsilon))
		{
			iter = o.mPosTrack.erase(iter);
		}
	}

	if (!o.mRotTrack.empty())
	{
		std::vector<AnimTrack>::iterator iter = o.mRotTrack.begin() + 1;
		for (;iter != o.mRotTrack.end() - 1;)
		{
			bool b0;
			bool b1;
			Quat r0 = (iter - 1)->mQuat;
			Quat r1 = iter->mQuat;
			Quat r2 = (iter + 1)->mQuat;

			b0 = r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon);
			b1 = r1.Equals(r2, Epsilon) || (-r1).Equals(r2, Epsilon);

			if (b0 && b1)
			{
				iter = o.mRotTrack.erase(iter);
			}
			else
				++iter;
		}

		iter = o.mRotTrack.end() - 1;

		Quat r0 = (iter - 1)->mQuat;
		Quat r1 = iter->mQuat;

		if (r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon))
		{
			iter = o.mRotTrack.erase(iter);
		}
	}

	if (!o.mScaleTrack.empty())
	{
		std::vector<AnimTrack>::iterator iter = o.mScaleTrack.begin() + 1;
		for (;iter != o.mScaleTrack.end() - 1;)
		{
			bool b0;
			bool b1;
			Quat r0 = (iter - 1)->mQuat;
			Quat r1 = iter->mQuat;
			Quat r2 = (iter + 1)->mQuat;

			b0 = r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon);
			b1 = r1.Equals(r2, Epsilon) || (-r1).Equals(r2, Epsilon);

			if (b0 &&
				b1 &&
				iter->mVec.Equals((iter - 1)->mVec, Epsilon) &&
				iter->mVec.Equals((iter + 1)->mVec, Epsilon))
			{
				iter = o.mScaleTrack.erase(iter);
			}
			else
				++iter;
		}

		iter = o.mScaleTrack.end() - 1;

		Quat r0 = (iter - 1)->mQuat;
		Quat r1 = iter->mQuat;

		if (r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon))
		{
			iter = o.mScaleTrack.erase(iter);
		}
	}
}
