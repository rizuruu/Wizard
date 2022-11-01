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

	Rect* AnimationFrames[1];
public:
	AnimationSequence(int argc, char* argv[]);
	void virtual PlaySequence(Sequence sequence);
};