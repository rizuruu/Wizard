#include "AIAgent.h"
#include <iostream>
#include "Pacman.h"
AIAgent::AIAgent()
{
	collision = new Collision(Collision::CollisionType::Dynamic);

	velocity = new Vector2();
	PrevPosition = new Vector2();
	rect = new Rect(1300, 500, 158, 100);
	collision->Rect = rect;

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
		CurrentState = AIState::Grounded;

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
		CurrentState = AIState::InAir;
		velocity->Y += 0.1f;
	}
	else
		velocity->Y = 0;

	cout << "distance: " << abs(rect->X - Pacman::Instance->_pacmanPosition->X) << endl;
	if (CurrentState == AIState::Grounded && abs(rect->X - Pacman::Instance->_pacmanPosition->X) < 500.0f)
		rect->X = MathHelper::Lerp(rect->X, Pacman::Instance->_pacmanPosition->X, 0.1f);

}

void AIAgent::DrawAI(AnimationSequence* sequence)
{
	sequence->PlaySequence(new Vector2(rect->X, rect->Y), false, 0.5f);
}