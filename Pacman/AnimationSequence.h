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
	Rect* AnimationFrames[100];
	vector<Rect> AnimFrames;
public:
	AnimationSequence();

	/// <summary>
	/// Initilize a single sprite sheet to vector/array of rects to animate. Takes all data as arguments
	/// </summary>
	/// <param name="Source">Texture2D Source of sprite sheet</param>
	/// <param name="FramesCount">Count of Frames of animation</param>
	/// <param name="grid">Grid vector of sprite sheet</param>
	/// <param name="width">Width of individual frame</param>
	/// <param name="height">Height of Individual Frame</param>
	void virtual Initialize(Texture2D* Source, int FramesCount, Vector2* grid, int width, int height);
	void virtual Initialize(Sequence sequence);

	/// <summary>
	/// Plays complete sequence as long as input is held
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="isFlipped"></param>
	void virtual PlaySequence(Vector2* pos, bool isFlipped, float scale = 1.0f);

	/// <summary>
	/// Plays complete sequence for single input press and returns a bool
	/// </summary>
	/// <param name="pos"></param>
	/// <param name="isFlipped"></param>
	/// <returns></returns>
	bool virtual PlaySequenceOnce(Vector2* pos, bool isFlipped, float scale = 1.0f);
};