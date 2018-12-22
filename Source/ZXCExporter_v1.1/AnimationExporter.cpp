#include "AnimationExporter.h"

std::unique_ptr<AnimationClip> AnimationExporter::LoadAnimation(INode** nodearr, size_t arraysize, std::unordered_map<std::wstring, size_t>& nodeIndex, TimeValue start, TimeValue end)
{
	std::unique_ptr<AnimationClip> clip = nullptr;
	for (size_t i = 0; i < arraysize; ++i)
	{
		size_t k = nodeIndex[nodearr[i]->GetName()];
		if (CheckForAnimation(nodearr[i], start, end))
		{
			if (clip == nullptr)
			{
				clip = std::make_unique<AnimationClip>();
				clip->BoneAnimations.resize(nodeIndex.size());
			}
			InputAnimations(nodearr[i], clip->BoneAnimations[k], start, end);
			OverlappedTrackErase(clip->BoneAnimations[k]);
		}
	}
	return std::move(clip);
}

bool AnimationExporter::CheckForAnimation(INode* node, TimeValue start, TimeValue end)
{
	AffineParts ap;
	Point3 firstPos, firstRotAxis, firstScaleFactor;
	float firstRotAngle;
	DecompAffine(start, node, ap, &firstRotAxis, &firstRotAngle);
	firstPos = ap.t;
	firstScaleFactor = ap.k;

	int tickInterval = GetTicksPerFrame();

	float rotAngle;
	Point3 rotAxis;

	for (TimeValue t = start + tickInterval; t <= end; t += tickInterval)
	{
		DecompAffine(t, node, ap, &rotAxis, &rotAngle);

		if (!ap.t.Equals(firstPos, Epsilon))
		{
			return true;
		}
		if (fabsf(rotAngle - firstRotAngle) > Epsilon)
		{
			return true;
		}
		else if (!firstRotAxis.Equals(rotAxis, Epsilon))
		{
			return true;
		}
		if (!ap.k.Equals(firstScaleFactor, Epsilon))
		{
			return true;
		}
	}
	return false;
}

void AnimationExporter::DecompAffine(TimeValue t, INode * node, AffineParts & ap, Point3 * rotAxis, float * rotAngle)
{
	Matrix3 tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
	decomp_affine(tm, &ap);

	if (rotAngle != nullptr)
		AngAxisFromQ(ap.q, rotAngle, *rotAxis);
}

void AnimationExporter::InputAnimations(INode * node, BoneAnimation& boneAni, TimeValue start, TimeValue end)
{
	AffineParts ap;
	int tickInterval = GetTicksPerFrame();

	for (TimeValue t = start; t <= end; t += tickInterval)
	{
		DecompAffine(t, node, ap);

		KeyFrame ani;
		ani.Tick = t;
		MaxUtil::ConvertVector(ap.t, ani.Translations);
		MaxUtil::ConvertVector(ap.q, ani.RotationQuat);
		MaxUtil::ConvertVector(ap.u, ani.ScaleQuat);
		MaxUtil::ConvertVector(ap.k, ani.Scale);
		boneAni.Keyframes.push_back(ani);
	}
}

void AnimationExporter::OverlappedTrackErase(BoneAnimation& boneAni)
{
	std::vector<KeyFrame>::iterator iter = std::next(boneAni.Keyframes.begin());

	Quat rot;
	Quat saxis;
	Point3 scale;
	Point3 trans;

	for (; iter != std::prev(boneAni.Keyframes.end());)
	{
		rot = iter->RotationQuat;
		saxis = iter->ScaleQuat;
		scale = iter->Scale;
		trans = iter->Translations;

		if (!isOverlapped(trans, std::prev(iter)->Translations, std::next(iter)->Translations))
		{
			++iter; continue;
		}

		if (!isOverlapped(scale, std::prev(iter)->Scale, std::next(iter)->Scale))
		{
			++iter; continue;
		}

		if (!isOverlappedQuat(rot, std::prev(iter)->RotationQuat, std::next(iter)->RotationQuat))
		{
			++iter; continue;
		}

		if (!isOverlappedQuat(saxis, std::prev(iter)->ScaleQuat, std::next(iter)->ScaleQuat))
		{
			++iter; continue;
		}


		iter = boneAni.Keyframes.erase(iter);
	}

	iter = std::prev(boneAni.Keyframes.end());

	rot = iter->RotationQuat;
	saxis = iter->ScaleQuat;
	scale = iter->Scale;
	trans = iter->Translations;

	if (!trans.Equals(std::prev(iter)->Translations))
		return;

	if (!rot.Equals(std::prev(iter)->RotationQuat) && !(-rot).Equals(std::prev(iter)->RotationQuat))
		return;

	if (!scale.Equals(std::prev(iter)->Scale))
		return;

	if (!saxis.Equals(std::prev(iter)->ScaleQuat) && !(-saxis).Equals(std::prev(iter)->ScaleQuat))
		return;

	iter = boneAni.Keyframes.erase(iter);
}
