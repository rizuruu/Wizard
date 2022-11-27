#include "Collision.h"
#include "AnimationSequence.h"

#pragma once
class AIAgent
{
public:
	AIAgent();
	~AIAgent();

	enum class AIState
	{
		Grounded,
		InAir,
	};

	AIState CurrentState = AIState::InAir;
	Vector2* velocity;
	Vector2* PrevPosition;
	Collision* collision;
	Rect* rect;
	

	void UpdateAI(int elapsedTime);
	void DrawAI(AnimationSequence* sequence);
};