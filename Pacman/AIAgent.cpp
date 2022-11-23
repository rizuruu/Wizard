#include "AIAgent.h"
#include <iostream>
AIAgent::AIAgent()
{
	collision = new Collision(Collision::CollisionType::Dynamic);

	velocity = new Vector2();
	rect = new Rect(650, 500, 128, 256);
	collision->Rect = rect;

}

AIAgent::~AIAgent()
{
	delete collision;
}

void AIAgent::UpdateAI(int elapsedTime)
{
	rect->X += velocity->X * elapsedTime;

	rect->Y += velocity->Y * elapsedTime;

	collision->Rect->X = rect->X;
	collision->Rect->Y = rect->Y;
	if (abs(collision->OverlapSize->X) > 0.0f || abs(collision->OverlapSize->Y) > 0.0f)
	{
		collision->OverlapSize->X = 0;

		collision->OverlapSize->Y = 0;

		velocity->Y = 0;
	}
	else
	{
		velocity->Y += 0.1f; //Gravity
	}
}

void AIAgent::DrawAI(int elapsedTime)
{
	SpriteBatch::DrawRectangle(rect, Color::White);
}