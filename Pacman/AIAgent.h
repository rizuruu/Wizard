#include "Collision.h"

#pragma once
class AIAgent
{
public:
	AIAgent();
	~AIAgent();

	Vector2* velocity;
	Collision* collision;
	Rect* rect;

	void UpdateAI(int elapsedTime);
	void DrawAI(int elapsedTime);
};