#include "S2D/S2D.h"
#include <list>
#include <vector>
#include "Sequence.h";
using namespace S2D;

#pragma once
class AnimationSequence
{

private:
	int CurrentFrame = 0;
	Vector2* _pacmanPosition;
	static const int t;
	Texture2D* SourceTexture;
	Rect* SourceRect;
	Vector2 v_FramesCount;
	int FramesCount;
	Rect* AnimationFrames[24];
public:
	AnimationSequence();
	void virtual PlaySequence(Sequence sequence);
	void virtual Initialize(Rect t);
};