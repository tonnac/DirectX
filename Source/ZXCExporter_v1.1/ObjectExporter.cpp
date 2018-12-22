#include "ZXCExporter.h"

void ObjectExporter::LoadObject(INode ** n, size_t arraysize,
	std::vector<std::unique_ptr<ZXCObject>>& ObjectList,
	std::unordered_map<std::wstring, size_t>& nodeIndex)
{
	ObjectList.resize(nodeIndex.size());
	for (size_t i = 0; i < arraysize; ++i)
	{
		std::unique_ptr<ZXCObject> maxObj = std::make_unique<ZXCObject>();
		size_t objIndex = nodeIndex[n[i]->GetName()];
		maxObj->mNodeName = std::make_pair(n[i]->GetName(), objIndex);
		INode* parent = n[i]->GetParentNode();

		maxObj->mParentName = std::make_pair(L"NONE", -1);
		if (parent != nullptr && (!parent->IsRootNode()))
		{
			maxObj->mParentName = std::make_pair(parent->GetName(), nodeIndex[parent->GetName()]);
			std::ReplaceString(maxObj->mParentName.first);
		}

		std::ReplaceString(maxObj->mNodeName.first);

		TimeValue t = mExporter->mInterval.Start();

		Matrix3 world = n[i]->GetNodeTM(t);
		MaxUtil::ConvertMatrix(world, maxObj->mWorld);
		Matrix3 painv = Inverse(parent->GetNodeTM(t));
		Matrix3 local = n[i]->GetNodeTM(t) * painv;

		AffineParts ap;
		decomp_affine(local, &ap);

		MaxUtil::ConvertVector(ap.t, maxObj->InititalPos.t);
		MaxUtil::ConvertVector(ap.q, maxObj->InititalPos.q);
		MaxUtil::ConvertVector(ap.u, maxObj->InititalPos.u);
		MaxUtil::ConvertVector(ap.k, maxObj->InititalPos.k);
		
		LoadMesh(n[i], maxObj.get());

		ObjectState os = n[i]->EvalWorldState(t);
		os.obj->SuperClassID();

		switch (os.obj->SuperClassID())
		{
		case GEOMOBJECT_CLASS_ID:
			maxObj->Type = ObjectType::MESH;
			break;
		case HELPER_CLASS_ID:
		{
			if (os.obj->ClassID() == Class_ID(BONE_CLASS_ID, 0))
				maxObj->Type = ObjectType::BONE;
			if (os.obj->ClassID() == Class_ID(DUMMY_CLASS_ID, 0))
				maxObj->Type = ObjectType::DUMMY;
			if (os.obj->ClassID() == BIPSLAVE_CONTROL_CLASS_ID || 
				os.obj->ClassID() == BIPBODY_CONTROL_CLASS_ID)
				maxObj->Type = ObjectType::BIPED;
		}
		break;
		default:
			break;
		}
		ObjectList[objIndex] = std::move(maxObj);
	}
	for (auto&x : ObjectList)
		BuildVBIB(x.get());
}

void ObjectExporter::LoadMesh(INode* node, ZXCObject* o)
{
	TimeValue t = mExporter->mInterval.Start();
	
	Matrix3 tm = node->GetObjTMAfterWSM(t);

	ObjectState os = node->EvalWorldState(t);
	if (os.obj == nullptr || os.obj->SuperClassID() != GEOMOBJECT_CLASS_ID)
	{
		Box3 box;
		os.obj->GetDeformBBox(t, box, &tm);
		MaxUtil::ConvertVector(box.pmin, o->mBoundingBox.pmin);
		MaxUtil::ConvertVector(box.pmax, o->mBoundingBox.pmax);
		return;
	}

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
			o->mTriangles[i].v[0].VertexNum = mesh.faces[i].v[i0];

			v = mesh.verts[mesh.faces[i].v[i2]] * tm * Inv;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[1].p);
			o->mTriangles[i].v[1].VertexNum = mesh.faces[i].v[i2];

			v = mesh.verts[mesh.faces[i].v[i1]] * tm * Inv;
			MaxUtil::ConvertVector(v, o->mTriangles[i].v[2].p);
			o->mTriangles[i].v[2].VertexNum = mesh.faces[i].v[i1];
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
		if ((int)mExporter->mOutputMaterial[o->mMaterialRef].SubMaterial.size() == 0)
		{
			o->mTriangles[i].mSubMtrl = -1;
		}
	}
	if (needDel)
		delete tri;
}

void ObjectExporter::BuildVBIB(ZXCObject* mesh)
{
	std::sort(mesh->mTriangles.begin(), mesh->mTriangles.end());
	for (int i = 0; i < (int)mesh->mTriangles.size(); ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			int mTlid = mesh->mTriangles[i].mSubMtrl;
			auto& vertices = mesh->mVertices[mTlid];
			auto& indices = mesh->mIndices[mTlid];
			int vNumber = [&](Vertex& rhs) -> int
			{
				for (int k = 0; k < (int)vertices.size(); ++k)
				{
					auto verts = vertices[k];
					if (verts == rhs)
					{
						return k;
					}
				}
				return -1;
			}(mesh->mTriangles[i].v[j]);
			if (vNumber == -1)
			{
				vertices.push_back(mesh->mTriangles[i].v[j]);
				indices.push_back((UINT)(vertices.size() - 1));
			}
			else
			{
				indices.push_back(vNumber);
			}
		}
	}

	for (auto &x : mesh->mIndices)
	{
		auto& indices = x.second;
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
	}
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
