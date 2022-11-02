#include "AnimationSequence.h"
using namespace std;

AnimationSequence::AnimationSequence()
{

}

void AnimationSequence::Initialize()
{
	int t = 0;

	for (int i = 0; i < v_FramesCount.Y; i++)
	{
		for (int j = 0; j < v_FramesCount.X; j++)
		{
			AnimationFrames[t] = new Rect(847.0f * j, 864.0f * i, 847, 864);
			t++;
		}
	}
}

void AnimationSequence::PlaySequence(Sequence sequence)
{
	FramesCount = sequence.FramesCount;

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