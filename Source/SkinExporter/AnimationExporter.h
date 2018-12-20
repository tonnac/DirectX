#pragma once

#include "Header.h"

class AnimationExporter
{
public:
	static void LoadAnimation(INode * node, ZXCObject & o, TimeValue start, TimeValue end);

private:
	static bool CheckForAnimation(INode* node, bool& isPos, bool& isRot, bool& isScale, TimeValue start, TimeValue end);
	static void DecompAffine(TimeValue t, INode * node, AffineParts& ap, Point3* rotAxis = nullptr, float* rotAngle = nullptr);
	
	static void InputPosSample(INode* node, ZXCObject& o, TimeValue start, TimeValue end);
	static void InputRotSample(INode* node, ZXCObject& o, TimeValue start, TimeValue end);
	static void InputSclaeSample(INode* node, ZXCObject& o, TimeValue start, TimeValue end);
	
	static void OverlappedTrackErase(ZXCObject& o);
};