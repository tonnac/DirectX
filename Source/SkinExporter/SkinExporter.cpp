#include "SkinExporter.h"

bool SkinExporter::isBipedObject(INode * node)
{
	Modifier * phyMod = FindModifer(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
	Modifier * skinMod = FindModifer(node, SKIN_CLASSID);

	if (phyMod != nullptr || skinMod != nullptr)
	{
		return true;
	}

	return false;
}

void SkinExporter::LoadBipedInfo(INode * node, VectorBiped* bipedes, TimeValue t)
{
	Modifier * phyMod = FindModifer(node, Class_ID(PHYSIQUE_CLASS_ID_A, PHYSIQUE_CLASS_ID_B));
	Modifier * skinMod = FindModifer(node, SKIN_CLASSID);

	if (phyMod != nullptr)
	{
		ExportPhysiqueData(node, bipedes, phyMod, t);
	}
	else if (skinMod != nullptr)
	{
		ExportSkinData(node, bipedes, skinMod);
	}
}

Modifier * SkinExporter::FindModifer(INode * node, Class_ID classID)
{
	Object * object = node->GetObjectRef();
	if (object == nullptr) return nullptr;

	while (object->SuperClassID() == GEN_DERIVOB_CLASS_ID && object)
	{
		IDerivedObject* DerivedObject = static_cast<IDerivedObject*>(object);

		int modStackIndex = 0;

		while (modStackIndex < DerivedObject->NumModifiers())
		{
			Modifier * modifier = DerivedObject->GetModifier(modStackIndex);

			if (modifier->ClassID() == classID)
				return modifier;

			++modStackIndex;
		}
		object = DerivedObject->GetObjRef();
	}
	return nullptr;
}

void SkinExporter::ExportPhysiqueData(INode * node, VectorBiped* bipedes, Modifier * phyMod, TimeValue t)
{
	IPhysiqueExport * phyExport = (IPhysiqueExport*)phyMod->GetInterface(I_PHYEXPORT);
	IPhyContextExport * mcExport = (IPhyContextExport *)phyExport->GetContextInterface(node);

	mcExport->ConvertToRigid(true);
	mcExport->AllowBlending(true);
	
	int numverts = mcExport->GetNumberVertices();

	ObjectState os = node->EvalWorldState(t);

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
