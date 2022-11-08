#include "AnimationSequence.h"
using namespace std;

AnimationSequence::AnimationSequence()
{

}

void AnimationSequence::Initialize(Texture2D* Source, int Frames, Vector2* grid)
{
	SourceTexture = Source;
	FramesCount = Frames;
	v_FramesCount = grid;
	int t = 0;
	for (int i = 0; i < v_FramesCount->Y; i++)
	{
		for (int j = 0; j < v_FramesCount->X; j++)
		{
			AnimationFrames[t] = new S2D::Rect(847.0f * j, 864.0f * i, 847, 864);
			t++;
		}
	}
}

void AnimationSequence::PlaySequence(Vector2* pos)
{
	try {
		CurrentFrame++;

		if (CurrentFrame < FramesCount)
		{
			SpriteBatch::Draw(SourceTexture, pos, AnimationFrames[CurrentFrame]);
		}
		else
		{
			CurrentFrame = 0;
			SpriteBatch::Draw(SourceTexture, pos, AnimationFrames[0]);
		}
	}
	catch(exception e)
	{
	}
}
