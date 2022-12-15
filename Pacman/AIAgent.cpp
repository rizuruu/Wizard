#include "AIAgent.h"
#include <iostream>
#include "GameManager.h"
AIAgent::AIAgent()
{
	collision = new Collision(Collision::CollisionType::Dynamic);

	velocity = new Vector2();
	PrevPosition = new Vector2();
	rect = new Rect(1300, 500, 170, 250);
	collision->Rect = rect;

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

	if (abs(rect->X - GameManager::Instance->WizardPosition->X) < 500.0f)
	{
		if (rect->X - GameManager::Instance->WizardPosition->X > 0)
			isFlipped = false;
		else 
			isFlipped = true;

		if (abs(rect->X - GameManager::Instance->WizardPosition->X) > 200.0f)
		{
			CurrentState = AIState::Walking;
			rect->X = MathHelper::Lerp(rect->X, GameManager::Instance->WizardPosition->X, 0.02f);
		}
		else
		{
			if (GameManager::Instance->PState == GameManager::PlayerState::Dead)
				CurrentState = AIState::Idle;
			else
				CurrentState = AIState::Attacking;
		}
	}
	else
		CurrentState = AIState::Idle;

	R_HealthBar->X = rect->X + 70;
	R_HealthBar->Y = rect->Y + 10;
}

void AIAgent::DrawAI(AnimationSequence* IdleSequence, AnimationSequence* WalkSequence, AnimationSequence* AttackSequence)
{
	//SpriteBatch::DrawRectangle(new Rect(rect->X, rect->Y + 10, 20, 30), Color::White); //0.443, 0.761, 0.788
	//SpriteBatch::DrawRectangle(new Rect(25, 30, 250, 20), new Color(0.0f, 0.0f, 0.0f, 0.5f)); //0.443, 0.761, 0.788
	SpriteBatch::DrawRectangle(R_HealthBar, new Color(0.0f, 0.0f, 0.0f, 0.5f)); //0.443, 0.761, 0.788
	SpriteBatch::DrawRectangle(R_HealthBar, Color::Green); //0.443, 0.761, 0.788

	switch (CurrentState)
	{
	case AIAgent::AIState::Idle:
		IdleSequence->PlaySequence(new Vector2(rect->X, rect->Y), isFlipped, 0.7f);
		break;
	case AIAgent::AIState::Walking:
		WalkSequence->PlaySequence(new Vector2(rect->X, rect->Y), isFlipped, 0.7f);
		break;
	case AIAgent::AIState::Attacking:
		if (AttackSequence->PlaySequenceOnce(new Vector2(rect->X, rect->Y), isFlipped, 0.7f))
		{
			GameManager::Instance->Damage(0.0f);
		}
		break;
	default:
		break;
	}
}

void AIAgent::Damage(float damage)
{
	if (Health > 0)
	{
		//PState = PlayerState::Damage;

		//float N_HealthBar = R_HealthBar->Width / 180.0f;
		Health -= 0.1f;
		R_HealthBar->Width = Health * 180.0f;
		cout << Health << endl;
	}
	else
	{
		//PState = PlayerState::Dead;
	}
}
