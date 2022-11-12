#include "AnimationSequence.h"
using namespace std;
#include <sstream>

AnimationSequence::AnimationSequence()
{

}

void AnimationSequence::Initialize(Texture2D* Source, int Frames, Vector2* grid, int width, int height)
{
	SourceTexture = Source;
	FramesCount = Frames;
	v_FramesCount = grid;

	for (int i = 0; i < v_FramesCount->Y; i++)
	{
		for (int j = 0; j < v_FramesCount->X; j++)
		{
			AnimFrames.push_back(Rect(width * j, height * i, width, height));
		}
	}
}

void AnimationSequence::Initialize(Sequence sequence)
{
	SourceTexture = sequence.Source;
	FramesCount = sequence.FramesCount;
	v_FramesCount = sequence.grid;

	for (int i = 0; i < v_FramesCount->Y; i++)
	{
		for (int j = 0; j < v_FramesCount->X; j++)
		{
			AnimFrames.push_back(Rect(sequence.width * j, sequence.height * i, sequence.width, sequence.height));
		}
	}
}

void AnimationSequence::PlaySequence(Vector2* pos, bool isFlipped, float scale)
{
	//std::stringstream stream;
	//stream << "frame: " << CurrentFrame;

	//SpriteBatch::DrawString(stream.str().c_str(), pos, Color::Green);

	if (CurrentFrame < FramesCount)
	{
		SpriteBatch::Draw(SourceTexture, pos, new Rect(AnimFrames.at(CurrentFrame)), Vector2::Zero, scale, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
		CurrentFrame++;
	}
	else
	{
		CurrentFrame = 0;
		SpriteBatch::Draw(SourceTexture, pos, new Rect(AnimFrames.at(0)), Vector2::Zero, scale, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
	}
}

bool AnimationSequence::PlaySequenceOnce(Vector2* pos, bool isFlipped)
{
	CurrentFrame++;

	if (CurrentFrame < FramesCount)
	{
		SpriteBatch::Draw(SourceTexture, pos, new Rect(AnimFrames.at(CurrentFrame)), Vector2::Zero, 0.5f, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
		return false;
	}
	else
	{
		CurrentFrame = 0;
		SpriteBatch::Draw(SourceTexture, pos, new Rect(AnimFrames.at(0)), Vector2::Zero, 0.5f, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
		return true;
	}
}
