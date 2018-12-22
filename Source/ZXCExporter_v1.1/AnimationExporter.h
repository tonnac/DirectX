#pragma once

#include "Header.h"

class AnimationExporter
{
public:
	static std::unique_ptr<AnimationClip> LoadAnimation(
		INode** nodearr, 
		size_t arraysize, 
		std::unordered_map<std::wstring, size_t>& nodeIndex,
		TimeValue start, TimeValue end);

private:
	static bool CheckForAnimation(INode* node, TimeValue start, TimeValue end);
	static void DecompAffine(TimeValue t, INode * node, AffineParts& ap, Point3* rotAxis = nullptr, float* rotAngle = nullptr);
	
	static void InputAnimations(INode* node, BoneAnimation& boneAni, TimeValue start, TimeValue end);

	static void OverlappedTrackErase(BoneAnimation& boneAni);

	template<typename X>
	static bool isOverlapped(const X& cen, const X& prev, const X& next)
	{
		if (cen.Equals(prev, Epsilon) &&
			cen.Equals(next, Epsilon))
		{
			return true;
		}
		return false;
	}

	static bool isOverlappedQuat(const Quat& cen, const Quat& prev, const Quat& next)
	{
		bool b0;
		bool b1;

		b0 = prev.Equals(cen, Epsilon) || (-prev).Equals(cen, Epsilon);
		b1 = cen.Equals(next, Epsilon) || (-cen).Equals(next, Epsilon);

		if (b0 &&
			b1)
		{
			return true;
		}
		return false;
	}
};