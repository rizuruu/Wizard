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
	Texture2D* SourceTexture;
	Rect* SourceRect;
	Vector2* v_FramesCount;
	int FramesCount;
	Rect* AnimationFrames[];
public:
	AnimationSequence();
	void virtual PlaySequence(Vector2* pos);
	void virtual Initialize(Texture2D* Source, int FramesCount, Vector2* grid);
};