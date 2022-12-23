#include "Collision.h"
#include "AnimationSequence.h"

#pragma once
class AIAgent
{
private:
	bool isFlipped;
	float Health;
public:
	enum class AI_Type
	{
		Trigger,
		Attack,
	};

	AIAgent();
	AIAgent(AI_Type Type, Vector2 pos = Vector2(1300, 500));
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
	AI_Type Enemy_Type = AI_Type::Attack;
	Vector2* velocity;
	Vector2* PrevPosition;
	Collision* collision;
	Rect* rect;
	Rect* R_HealthBar;

	void UpdateAI(int elapsedTime);
	void DrawAI();
	void Damage(float damage);
};