#include "AIAgent.h"
#include <iostream>
#include "Pacman.h"
AIAgent::AIAgent()
{
	collision = new Collision(Collision::CollisionType::Dynamic);

	velocity = new Vector2();
	PrevPosition = new Vector2();
	rect = new Rect(650, 500, 128, 256);
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
	PrevPosition->X = rect->X;
	PrevPosition->Y = rect->Y;

	rect->X += velocity->X * elapsedTime;

	rect->Y += velocity->Y * elapsedTime;

	collision->Rect->X = rect->X;
	collision->Rect->Y = rect->Y;
	if (abs(collision->OverlapSize->X) > 0.0f || abs(collision->OverlapSize->Y) > 0.0f)
	{
		rect->Y = PrevPosition->Y -1;


		collision->OverlapSize->X = 0;

		collision->OverlapSize->Y = 0;

		velocity->Y = 0;
		CurrentState = AIState::Grounded;
	}
	else
	{
		CurrentState = AIState::InAir;

		velocity->Y += 0.1f; //Gravity
	}
	
	if (CurrentState == AIState::Grounded)
		rect->X = MathHelper::Lerp(rect->X, Pacman::Instance->_pacmanPosition->X, 0.1f);

}

void AIAgent::DrawAI(int elapsedTime)
{
	SpriteBatch::DrawRectangle(rect, Color::White);
}