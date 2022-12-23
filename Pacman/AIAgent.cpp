#include "AIAgent.h"
#include <iostream>
#include "GameManager.h"
AIAgent::AIAgent(AI_Type Type, Vector2 pos)
{
	collision = new Collision(Collision::CollisionType::Dynamic);
	Enemy_Type = Type;
	velocity = new Vector2();
	PrevPosition = new Vector2();
	rect = new Rect(pos.X, pos.Y, 250, 250);
	collision->Rect = rect;
	collision->Rect->X = rect->X + 100;

	R_HealthBar = new Rect(rect->X + 70, rect->Y + 10, 180, 12);
	Health = float(1.0f);
	isFlipped = true;
}

AIAgent::~AIAgent()
{
	delete collision;
	delete velocity;
	delete rect;
	delete PrevPosition;
}

void AIAgent::UpdateAI(int elapsedTime)
{ 
	bool wasOverlapping = false;
	PrevPosition->X = rect->X;
	PrevPosition->Y = rect->Y;

	rect->X += velocity->X * elapsedTime;

	rect->Y += velocity->Y * elapsedTime;

	collision->Rect->X = rect->X;
	collision->Rect->Y = rect->Y;
	if (abs(collision->OverlapSize->X) > 0.0f || abs(collision->OverlapSize->Y) > 0.0f)
	{
		if (abs(collision->OverlapSize->X) > 0.0f)
			wasOverlapping = true;

		if (abs(collision->OverlapSize->X) >= collision->Rect->Width)
		{
			while (CollisionManager::Instance->IsCollider(Vector2(rect->X, rect->Y + rect->Height)) ||
				CollisionManager::Instance->IsCollider(Vector2(rect->X + rect->Width, rect->Y + rect->Height)))
			{
				rect->Y--;
				velocity->Y = 0;
				collision->Rect->X = rect->X;
				collision->Rect->Y = rect->Y;
			}
		}

		while (CollisionManager::Instance->IsCollider(Vector2(rect->X, rect->Y + 1.0f)) ||
			CollisionManager::Instance->IsCollider(Vector2(rect->X + rect->Width, rect->Y + 1.0f)))
		{
			rect->Y++;
			velocity->Y = 0;
			collision->Rect->X = rect->X;
			collision->Rect->Y = rect->Y;
		}

	}

	if (!CollisionManager::Instance->IsCollider(Vector2(rect->X, rect->Y + rect->Height + 1.0f)) &&
		!CollisionManager::Instance->IsCollider(Vector2(rect->X + rect->Width, rect->Y + rect->Height + 1.0f)))
	{
		velocity->Y += 0.1f;
	}
	else
		velocity->Y = 0;

	if (abs(rect->X - GameManager::Instance->WitchPosition->X) < 500.0f && abs(rect->Y - GameManager::Instance->WitchPosition->Y) < 100.0f)
	{
		if (CurrentState == AIState::Dead)
			return;

		if (rect->X - GameManager::Instance->WitchPosition->X > 0)
			isFlipped = false;
		else 
			isFlipped = true;

		if (abs(rect->X - GameManager::Instance->WitchPosition->X) > 200.0f && Enemy_Type == AI_Type::Attack)
		{
			CurrentState = AIState::Walking;
			rect->X = MathHelper::Lerp(rect->X, GameManager::Instance->WitchPosition->X, 0.02f);
		}
		else
		{
			if (GameManager::Instance->PState == GameManager::PlayerState::Dead)
				CurrentState = AIState::Idle;
			else if (Enemy_Type == AI_Type::Attack)
				CurrentState = AIState::Attacking;
			else
			{
				CurrentState = AIState::Idle;
			}
		}
	}
	else
	{
		if (CurrentState == AIState::Dead)
			return;
		else
			CurrentState = AIState::Idle;
	}

	R_HealthBar->X = rect->X + 70;
	R_HealthBar->Y = rect->Y + 10;
}

void AIAgent::DrawAI()
{
	SpriteBatch::DrawRectangle(R_HealthBar, new Color(0.0f, 0.0f, 0.0f, 0.5f)); 
	SpriteBatch::DrawRectangle(R_HealthBar, new Color(1.2 - Health, Health, 0.0f));

	switch (CurrentState)
	{
	case AIAgent::AIState::Idle:
		A_Idle->PlaySequence(new Vector2(rect->X, rect->Y), isFlipped, 0.7f);
		break;
	case AIAgent::AIState::Walking:
		A_Walk->PlaySequence(new Vector2(rect->X, rect->Y), isFlipped, 0.7f);
		break;
	case AIAgent::AIState::Attacking:
		if (A_Attack->PlaySequenceOnce(new Vector2(rect->X, rect->Y), isFlipped, 0.7f))
		{
			GameManager::Instance->Damage(0.0f);
		}
		break;
	case AIAgent::AIState::Dead:
		SpriteBatch::Draw(T_Dead, new Vector2(rect->X, rect->Y), new Rect(0, 0, 480, 480), Vector2::Zero, 0.7f, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
		break;
	default:
		break;
	}
}

void AIAgent::Damage(float damage)
{
	if (abs(rect->X - GameManager::Instance->WitchPosition->X) <= 200.0f)
	{
		if (Health > 0)
		{
			Health -= 0.1f;
			R_HealthBar->Width = Health * 180.0f;
		}
		else
		{
			CurrentState = AIState::Dead;
		}
	}
}
