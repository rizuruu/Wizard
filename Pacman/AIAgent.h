#include "Collision.h"
#include "AnimationSequence.h"

#pragma once
class AIAgent
{
private:
	bool isFlipped;
	float Health;
public:
	AIAgent();
	~AIAgent();

	enum class AIState
	{
		Idle,
		Walking,
		Attacking,
		Dead,
	};

	Texture2D* T_Idle;
	Texture2D* T_Walk;
	Texture2D* T_Attack;
	Texture2D* T_Dead;

	AnimationSequence* A_Idle;
	AnimationSequence* A_Walk;
	AnimationSequence* A_Attack;
	AnimationSequence* A_Dead;

	AIState CurrentState = AIState::Idle;
	Vector2* velocity;
	Vector2* PrevPosition;
	Collision* collision;
	Rect* rect;
	Rect* R_HealthBar;

	void UpdateAI(int elapsedTime);
	void DrawAI(AnimationSequence* IdleSequence, AnimationSequence* WalkSequence, AnimationSequence* AttackSequence);
	void Damage(float damage);
};