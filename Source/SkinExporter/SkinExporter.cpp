#include "ZXCExporter.h"

SkinExporter::SkinExporter(ZXCExporter * exporter)
	: ObjectExporter(exporter)
{
}

void SkinExporter::LoadBipedInfo(INode * node, VectorBiped* bipedes)
{
	Modifier * phyMod = MaxUtil::FindModifer(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
	Modifier * skinMod = MaxUtil::FindModifer(node, SKIN_CLASSID);

	if (phyMod != nullptr)
	{
		ExportPhysiqueData(node, bipedes, phyMod);
	}
	else if (skinMod != nullptr)
	{
		ExportSkinData(node, bipedes, skinMod);
	}
}

void SkinExporter::LoadMesh(INode * node, ZXCObject * o)
{
	SkinMesh* skinMesh = static_cast<SkinMesh*>(o);

	skinMesh->isBipedObject = true;

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
	MaxUtil::ConvertVector(box.pmin, skinMesh->mBoundingBox.pmin);
	MaxUtil::ConvertVector(box.pmax, skinMesh->mBoundingBox.pmax);

	std::uint32_t i0, i1, i2;
	i0 = 0, i1 = 1, i2 = 2;
	if (MaxUtil::isTMNegative(tm))
	{
		i0 = 2, i1 = 1, i2 = 0;
	}

	skinMesh->mMaterialRef = GetMaterialRef(node->GetMtl());
	skinMesh->mSkinTriList.resize(mesh.getNumFaces());
	for (int i = 0; i < (int)skinMesh->mSkinTriList.size(); ++i)
	{
		Matrix3 Inv = Inverse(node->GetNodeTM(t));

		Point3 v;

		std::unique_ptr<VectorBiped> biped = std::make_unique<VectorBiped>();
		LoadBipedInfo(node, biped.get());

		if (mesh.getNumVerts() > 0)
		{
			v = mesh.verts[mesh.faces[i].v[i0]] * tm * Inv;
			MaxUtil::ConvertVector(v, skinMesh->mSkinTriList[i].v[0].p);

			v = mesh.verts[mesh.faces[i].v[i2]] * tm * Inv;
			MaxUtil::ConvertVector(v, skinMesh->mSkinTriList[i].v[1].p);

			v = mesh.verts[mesh.faces[i].v[i1]] * tm * Inv;
			MaxUtil::ConvertVector(v, skinMesh->mSkinTriList[i].v[2].p);

			if (!(*biped).empty())
			{
				InputBipedes(skinMesh->mSkinTriList[i].v[0], (*biped)[mesh.faces[i].v[i0]]);
				InputBipedes(skinMesh->mSkinTriList[i].v[1], (*biped)[mesh.faces[i].v[i2]]);
				InputBipedes(skinMesh->mSkinTriList[i].v[2], (*biped)[mesh.faces[i].v[i1]]);
			}
		}
		if (mesh.getNumTVerts() > 0)
		{
			skinMesh->mSkinTriList[i].v[0].t.x = mesh.tVerts[mesh.tvFace[i].t[i0]].x;
			skinMesh->mSkinTriList[i].v[0].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i0]].y;

			skinMesh->mSkinTriList[i].v[1].t.x = mesh.tVerts[mesh.tvFace[i].t[i2]].x;
			skinMesh->mSkinTriList[i].v[1].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i2]].y;

			skinMesh->mSkinTriList[i].v[2].t.x = mesh.tVerts[mesh.tvFace[i].t[i1]].x;
			skinMesh->mSkinTriList[i].v[2].t.y = 1.0f - mesh.tVerts[mesh.tvFace[i].t[i1]].y;
		}
		if (mesh.getNumVertCol() > 0)
		{
			CopyMemory(&skinMesh->mSkinTriList[i].v[0].c, &mesh.vertCol[mesh.vcFace[i].t[i0]], sizeof(VertColor));
			CopyMemory(&skinMesh->mSkinTriList[i].v[1].c, &mesh.vertCol[mesh.vcFace[i].t[i2]], sizeof(VertColor));
			CopyMemory(&skinMesh->mSkinTriList[i].v[2].c, &mesh.vertCol[mesh.vcFace[i].t[i1]], sizeof(VertColor));
		}

		mesh.buildNormals();

		int vert = mesh.faces[i].getVert(i0);
		Point3 vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, skinMesh->mSkinTriList[i].v[0].n);

		vert = mesh.faces[i].getVert(i2);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, skinMesh->mSkinTriList[i].v[1].n);

		vert = mesh.faces[i].getVert(i1);
		vn = GetVertexNormal(mesh, i, mesh.getRVert(vert));
		MaxUtil::ConvertVector(vn, skinMesh->mSkinTriList[i].v[2].n);

		skinMesh->mSkinTriList[i].mSubMtrl = mesh.faces[i].getMatID();
	}
	if (needDel)
		delete tri;
}

std::unique_ptr<ZXCObject> SkinExporter::MakeObject()
{
	return std::move(std::make_unique<SkinMesh>());
}

void SkinExporter::InputBipedes(PNCTW4VERTEX & vertex, const BipedVertex & bipedes)
{
	int i = 0;
	for (auto & x : bipedes.mWeightList)
	{
#if MAXWEIGHTNUM == 8
#else
		vertex.w[i] = x.first;
		vertex.i[i] = x.second;
#endif
		++i;
	}
}

void SkinExporter::ExportPhysiqueData(INode * node, VectorBiped* bipedes, Modifier * phyMod)
{
	IPhysiqueExport * phyExport = (IPhysiqueExport*)phyMod->GetInterface(I_PHYEXPORT);
	IPhyContextExport * mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);

	mcExport->ConvertToRigid(true);
	mcExport->AllowBlending(true);
	
	int numverts = mcExport->GetNumberVertices();
	
	ObjectState os = node->EvalWorldState(mExporter->mInterval.Start());

	for (int i = 0; i < numverts; ++i)
	{
		float totalWeight = 0.0f, wieght = 0.0f;
		TSTR nodeName;

		IPhyVertexExport * vi = mcExport->GetVertexInterface(i);
		if (vi == nullptr)
		{
			continue;
		}

		int type = vi->GetVertexType();

		switch (type)
		{
		case RIGID_BLENDED_TYPE:
		{
		IPhyBlendedRigidVertex* rb_vtx = (IPhyBlendedRigidVertex*)vi;

		int iNode = rb_vtx->GetNumberNodes();
		BipedVertex bipVertex;
		bipVertex.mNumWeight = iNode;

		if (iNode > MAXWEIGHTNUM)
		{
			WeightMap weightList;
			bipVertex.mNumWeight = MAXWEIGHTNUM;
			for (int j = 0; j < iNode; ++j)
			{
				INode* node0 = rb_vtx->GetNode(j);
				nodeName = node0->GetName();

				float weight = rb_vtx->GetWeight(j);

				auto pair = std::make_pair(weight, std::wstring(nodeName));
				weightList.insert(pair);
			}
			int Count = 0;
			for (WeightIter iter = weightList.cbegin(); Count < MAXWEIGHTNUM; ++Count, ++iter)
			{
				bipVertex.mWeightList.insert(std::make_pair(iter->first, iter->second));
			}
		}
		else
		{
			for (int j = 0; j < iNode; ++j)
			{
				INode* node0 = rb_vtx->GetNode(j);
				nodeName = node0->GetName();

				float weight = rb_vtx->GetWeight(j);

				auto pair = std::make_pair(weight, std::wstring(nodeName));
				bipVertex.mWeightList.insert(pair);
			}
		}
		bipVertex.mNodename = node->GetName();
		bipedes->push_back(bipVertex);
		}break;
		case RIGID_NON_BLENDED_TYPE:
		{
			IPhyRigidVertex* r_vtx = (IPhyRigidVertex*)vi;

			INode * node0 = r_vtx->GetNode();

			BipedVertex bipVertex;

			bipVertex.mNumWeight = 1;

			std::wstring name = node0->GetName();
			bipVertex.mWeightList.insert(std::make_pair(1.0f, name));

			bipVertex.mNodename = node->GetName();
			bipedes->push_back(bipVertex);
		}break;
		default:
			MessageBox(nullptr, L"No Biped ID.", L"Error", 0);
			break;
		}
		mcExport->ReleaseVertexInterface(vi);
	}
	phyExport->ReleaseContextInterface(mcExport);
	phyMod->ReleaseInterface(I_PHYINTERFACE, phyExport);
}

void SkinExporter::ExportSkinData(INode * node, VectorBiped* bipedes, Modifier * skinMod)
{
	if (node == nullptr || skinMod == nullptr) return;

	ISkin * skin = (ISkin*)skinMod->GetInterface(I_SKIN);
	ISkinContextData * skinData = skin->GetContextInterface(node);

	if (skin != nullptr && skinData != nullptr)
	{
		int numOfPoints = skinData->GetNumPoints();

		for (int i = 0; i < numOfPoints; ++i)
		{
			int numOfWeights = skinData->GetNumAssignedBones(i);

			BipedVertex bipVertex;
			bipVertex.mNumWeight = numOfWeights;

			if (numOfWeights > MAXWEIGHTNUM)
			{
				WeightMap weightList;
				for (int j = 0; j < numOfWeights; ++j)
				{
					int boneIndex = skinData->GetAssignedBone(i, j);
					INode * bone = skin->GetBone(boneIndex);
					if (bone == nullptr) break;

					float boneWeight = skinData->GetBoneWeight(i, j);

					weightList.insert(std::make_pair(boneWeight, std::wstring(bone->GetName())));
				}

				int Cnt = 0;
				for (WeightIter iter = weightList.cbegin();Cnt < MAXWEIGHTNUM; ++Cnt, ++iter)
				{
					bipVertex.mWeightList.insert(std::make_pair(iter->first, iter->second));
				}
				bipVertex.mNumWeight = MAXWEIGHTNUM;
			}
			else
			{
				for (int j = 0; j < numOfWeights; ++j)
				{
					int boneIndex = skinData->GetAssignedBone(i, j);
					INode * bone = skin->GetBone(boneIndex);
					if (bone == nullptr) break;

					float boneWeight = skinData->GetBoneWeight(i, j);

					bipVertex.mWeightList.insert(std::make_pair(boneWeight, std::wstring(bone->GetName())));
				}
			}
			bipVertex.mNodename = node->GetName();
		}
	}
}
