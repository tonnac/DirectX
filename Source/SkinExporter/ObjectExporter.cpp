#include "ZXCExporter.h"

ZXCMap ObjectExporter::LoadObject(const std::vector<INode*> objects)
{
	ZXCMap ObjectList;
	for (auto& x : objects)
	{
		std::unique_ptr<ZXCObject> maxObj = MakeObject();
		maxObj->mNodeName = x->GetName();
		INode* parent = x->GetParentNode();

		maxObj->mParentName = L"NONE";
		if (parent != nullptr && (!parent->IsRootNode()))
		{
			maxObj->mParentName = parent->GetName();
			std::ReplaceString(maxObj->mParentName);
		}

		std::ReplaceString(maxObj->mNodeName);

		TimeValue t = mExporter->mInterval.Start();

		Matrix3 world = x->GetNodeTM(t);
		MaxUtil::ConvertMatrix(world, maxObj->mWorld);

		LoadMesh(x, maxObj.get());
		AnimationExporter::LoadAnimation(x, maxObj.get(), t, mExporter->mInterval.End());

		ObjectState os = x->EvalWorldState(t);
		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
		case HELPER_CLASS_ID:
			ObjectList.insert(std::make_pair(maxObj->mNodeName, std::move(maxObj)));
			break;
		default:
			break;
		}
	}
	return ObjectList;
}

std::unique_ptr<ZXCObject> ObjectExporter::MakeObject()
{
	return std::move(std::make_unique<ZXCObject>());
}

void ObjectExporter::LoadMesh(INode* node, ZXCObject* o)
{
	TimeValue t = mExporter->mInterval.Start();

	Matrix3 tm = node->GetObjTMAfterWSM(t);

	ObjectState os = node->EvalWorldState(t);
	if (os.obj == nullptr || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
		return;

	bool needDel;
	TriObject * tri = GetTriObject(os.obj, t, needDel);
	if (tri == nullptr) return;

	Mesh mesh = tri->GetMesh();
	mesh.buildBoundingBox();
	Box3 box = mesh.getBoundingBox(&tm);
	MaxUtil::ConvertVector(box.pmin, o->mBoundingBox.pmin);
	MaxUtil::ConvertVector(box.pmax, o->mBoundingBox.pmax);

	std::uint32_t i0, i1, i2;
	i0 = 0, i1 = 1, i2 = 2;
	if (MaxUtil::isTMNegative(tm))
	{
		i0 = 2, i1 = 1, i2 = 0;
	}

	o->mMaterialRef = GetMaterialRef(node->GetMtl());
	o->mTriangles.resize(mesh.getNumFaces());
	for (int i = 0; i < (int)o->mTriangles.size(); ++i)
	{
		Matrix3 Inv = Inverse(node->GetNodeTM(t));
		
		Point3 v;

		if (mesh.getNumVerts() > 0)
		{
			v = mesh.verts[mesh.faces[i].v[i0]] * tm * Inv;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[0].p);

			v = mesh.verts[mesh.faces[i].v[i2]] * tm * Inv;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[1].p);

			v = mesh.verts[mesh.faces[i].v[i1]] * tm * Inv;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[2].p);
		}
		if (mesh.getNumTVerts() > 0)
		{
			o->mTriangles[i].v[0].t.x = mesh.tVerts[mesh.tvFace[i].t[i0]].x;
			o->mTriangles[i].v[0].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i0]].y;

			o->mTriangles[i].v[1].t.x = mesh.tVerts[mesh.tvFace[i].t[i2]].x;
			o->mTriangles[i].v[1].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i2]].y;
			 
			o->mTriangles[i].v[2].t.x = mesh.tVerts[mesh.tvFace[i].t[i1]].x;
			o->mTriangles[i].v[2].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i1]].y;
		}
		if (mesh.getNumVertCol() > 0)
		{
			CopyMemory(&o->mTriangles[i].v[0].c, &mesh.vertCol[mesh.vcFace[i].t[i0]], sizeof(VertColor));
			CopyMemory(&o->mTriangles[i].v[1].c, &mesh.vertCol[mesh.vcFace[i].t[i2]], sizeof(VertColor));
			CopyMemory(&o->mTriangles[i].v[2].c, &mesh.vertCol[mesh.vcFace[i].t[i1]], sizeof(VertColor));
		}

		mesh.buildNormals();

		int vert = mesh.faces[i].getVert(i0);
		Point3 vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[0].n);

		vert = mesh.faces[i].getVert(i2);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[1].n);

		vert = mesh.faces[i].getVert(i1);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, o->mTriangles[i].v[2].n);

		o->mTriangles[i].mSubMtrl = mesh.faces[i].getMatID();
	}
	if (needDel)
		delete tri;
}

TriObject * ObjectExporter::GetTriObject(Object* obj, TimeValue t, bool & isDelete)
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

Point3 ObjectExporter::GetVertexNormal(Mesh & mesh, int faceNo, const RVertex & rv)
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

int ObjectExporter::GetMaterialRef(Mtl * mtl)
{
	for (int i = 0; i < (int)mExporter->mMaxMaterial.size(); ++i)
	{
		if (mExporter->mMaxMaterial[i] == mtl)
		{
			return i;
		}
	}
	return -1;
}

ObjectExporter::ObjectExporter(ZXCExporter * exporter)
	: mExporter(exporter)
{
}
