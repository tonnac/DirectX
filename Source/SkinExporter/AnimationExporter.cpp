#include "AnimationExporter.h"

void AnimationExporter::LoadAnimation(INode * node, ZXCObject * o, TimeValue start, TimeValue end)
{
	bool isPosAnimation;
	bool isRotAnimation;
	bool isScaleAnimation;

	if (CheckForAnimation(node, isPosAnimation, isRotAnimation, isScaleAnimation, start, end))
	{
		if (isPosAnimation)
		{
			InputPosSample(node, o, start, end);
		}

		if (isRotAnimation)
		{
			InputRotSample(node, o, start, end);
		}

		if (isScaleAnimation)
		{
			InputSclaeSample(node, o, start, end);
		}

		OverlappedTrackErase(o);
	}
}

bool AnimationExporter::CheckForAnimation(INode* node, bool& isPos, bool& isRot, bool& isScale, TimeValue start, TimeValue end)
{
	AffineParts ap;
	Point3 firstPos, firstRotAxis, firstScaleFactor;
	float firstRotAngle;
	DecompAffine(start, node, ap, &firstRotAxis, &firstRotAngle);
	firstPos = ap.t;
	firstScaleFactor = ap.k;

	int tickInterval = GetTicksPerFrame();

	isPos = isRot = isScale = false;
	float rotAngle;
	Point3 rotAxis;

	for (TimeValue t = start + tickInterval; t <= end; t += tickInterval)
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
			else if (!firstRotAxis.Equals(rotAxis, Epsilon))
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

void AnimationExporter::DecompAffine(TimeValue t, INode * node, AffineParts & ap, Point3 * rotAxis, float * rotAngle)
{
	Matrix3 tm = node->GetNodeTM(t) * Inverse(node->GetParentTM(t));
	decomp_affine(tm, &ap);

	if (rotAngle != nullptr)
		AngAxisFromQ(ap.q, rotAngle, *rotAxis);
}

void AnimationExporter::InputPosSample(INode* node, ZXCObject* o, TimeValue start, TimeValue end)
{
	AffineParts ap;
	int tickInterval = GetTicksPerFrame();

	for (TimeValue t = start; t <= end; t += tickInterval)
	{
		DecompAffine(t, node, ap);

		AnimTrack ani;
		ani.mTick = t;
		MaxUtil::ConvertVector(ap.t, ani.mVec);
		o->mPosTrack.push_back(ani);
	}
}

void AnimationExporter::InputRotSample(INode* node, ZXCObject* o, TimeValue start, TimeValue end)
{
	AffineParts ap;
	int tickInterval = GetTicksPerFrame();

	for (TimeValue t = start; t <= end; t += tickInterval)
	{
		DecompAffine(t, node, ap);

		AnimTrack ani;
		ani.mTick = t;
		MaxUtil::ConvertVector(ap.q, ani.mQuat);
		o->mRotTrack.push_back(ani);
	}
}

void AnimationExporter::InputSclaeSample(INode* node, ZXCObject* o, TimeValue start, TimeValue end)
{
	AffineParts ap;
	int tickInterval = GetTicksPerFrame();

	for (TimeValue t = start; t <= end; t += tickInterval)
	{
		DecompAffine(t, node, ap);

		AnimTrack ani;
		ani.mTick = t;
		MaxUtil::ConvertVector(ap.u, ani.mQuat);
		MaxUtil::ConvertVector(ap.k, ani.mVec);
		o->mScaleTrack.push_back(ani);
	}
}

void AnimationExporter::OverlappedTrackErase(ZXCObject * o)
{
	if (!o->mPosTrack.empty())
	{
		std::vector<AnimTrack>::iterator iter = std::next(o->mPosTrack.begin());
		for (;iter != std::prev(o->mPosTrack.end());)
		{
			if (iter->mVec.Equals(std::prev(iter)->mVec, Epsilon) &&
				iter->mVec.Equals(std::next(iter)->mVec, Epsilon))
			{
				iter = o->mPosTrack.erase(iter);
			}
			else
				++iter;
		}

		iter = std::prev(o->mPosTrack.end());

		Quat r0 = std::prev(iter)->mQuat;
		Quat r1 = iter->mQuat;

		if (r0.Equals(r1, Epsilon))
		{
			iter = o->mPosTrack.erase(iter);
		}
	}

	if (!o->mRotTrack.empty())
	{
		std::vector<AnimTrack>::iterator iter = std::next(o->mRotTrack.begin());
		for (;iter != std::prev(o->mRotTrack.end());)
		{
			bool b0;
			bool b1;
			Quat r0 = std::prev(iter)->mQuat;
			Quat r1 = iter->mQuat;
			Quat r2 = std::next(iter)->mQuat;

			b0 = r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon);
			b1 = r1.Equals(r2, Epsilon) || (-r1).Equals(r2, Epsilon);

			if (b0 && b1)
			{
				iter = o->mRotTrack.erase(iter);
			}
			else
				++iter;
		}

		iter = std::prev(o->mRotTrack.end());

		Quat r0 = std::prev(iter)->mQuat;
		Quat r1 = iter->mQuat;

		if (r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon))
		{
			iter = o->mRotTrack.erase(iter);
		}
	}

	if (!o->mScaleTrack.empty())
	{
		std::vector<AnimTrack>::iterator iter = std::next(o->mScaleTrack.begin());
		for (;iter != std::prev(o->mScaleTrack.end());)
		{
			bool b0;
			bool b1;
			Quat r0 = std::prev(iter)->mQuat;
			Quat r1 = iter->mQuat;
			Quat r2 = std::next(iter)->mQuat;

			b0 = r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon);
			b1 = r1.Equals(r2, Epsilon) || (-r1).Equals(r2, Epsilon);

			if (b0 &&
				b1 &&
				iter->mVec.Equals(std::prev(iter)->mVec, Epsilon) &&
				iter->mVec.Equals(std::next(iter)->mVec, Epsilon))
			{
				iter = o->mScaleTrack.erase(iter);
			}
			else
				++iter;
		}

		iter = std::prev(o->mScaleTrack.end());

		Quat r0 = std::prev(iter)->mQuat;
		Quat r1 = iter->mQuat;

		if (r0.Equals(r1, Epsilon) || (-r0).Equals(r1, Epsilon))
		{
			iter = o->mScaleTrack.erase(iter);
		}
	}
}
