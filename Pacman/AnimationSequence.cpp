#include "AnimationSequence.h"
using namespace std;

AnimationSequence::AnimationSequence(int argc, char* argv[])
{
	CurrentFrame = 0;
	v_FramesCount = S2D::Vector2(6, 4);
	
}

void AnimationSequence::PlaySequence(Sequence sequence)
{

	SourceRect = sequence.GridVector;
	AnimationFrames = new S2D::Rect[sequence.FramesCount];

	if (CurrentFrame < size(AnimationFrames))
	{
		SpriteBatch::Draw(SourceTexture, _pacmanPosition, AnimationFrames[CurrentFrame]);
	}
	else
	{
		CurrentFrame = 0;
		SpriteBatch::Draw(SourceTexture, _pacmanPosition, AnimationFrames[0]);
	}
}