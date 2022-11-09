#include "AnimationSequence.h"
using namespace std;
#include <sstream>

AnimationSequence::AnimationSequence()
{

}

void AnimationSequence::Initialize(Texture2D* Source, int Frames, Vector2* grid)
{
	SourceTexture = Source;
	FramesCount = Frames;
	v_FramesCount = grid;

	for (int i = 0; i < v_FramesCount->Y; i++)
	{
		for (int j = 0; j < v_FramesCount->X; j++)
		{
			AnimFrames.push_back(Rect(512.0f * j, 512.0f * i, 512, 512));
			//AnimationFrames[t] = new S2D::Rect(847.0f * j, 864.0f * i, 847, 864);
		}
	}
}

void AnimationSequence::Initialize(Texture2D* Source, int Frames, Vector2* grid, int w)
{
	SourceTexture = Source;
	FramesCount = Frames;
	v_FramesCount = grid;

	for (int i = 0; i < v_FramesCount->Y; i++)
	{
		for (int j = 0; j < v_FramesCount->X; j++)
		{
			AnimFrames.push_back(Rect(w * j, w * i, w, w));
			//AnimationFrames[t] = new S2D::Rect(847.0f * j, 864.0f * i, 847, 864);
		}
	}
}

void AnimationSequence::PlaySequence(Vector2* pos, bool isFlipped)
{
	std::stringstream stream;
	stream << "frame: " << CurrentFrame;

	SpriteBatch::DrawString(stream.str().c_str(), pos, Color::Green);

	if (CurrentFrame < FramesCount)
	{
		SpriteBatch::Draw(SourceTexture, pos, new Rect(AnimFrames.at(CurrentFrame)), Vector2::Zero, 1.0f, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
		CurrentFrame++;
	}
	else
	{
		CurrentFrame = 0;
		SpriteBatch::Draw(SourceTexture, pos, new Rect(AnimFrames.at(0)), Vector2::Zero, 1.0f, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
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
