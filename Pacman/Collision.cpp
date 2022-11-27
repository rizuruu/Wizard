#include "Collision.h"
#include "CollisionManager.h"
#include <iostream>
#include "S2D/S2D.h"
using namespace S2D;

Collision::Collision(CollisionType Type)
{
	Rect = new S2D::Rect();
	OverlapSize = new Vector2();
	if (CollisionManager::Instance != NULL)
	{
		this->Type = Type;
		CollisionManager::Instance->Collisions->push_back(this);
	}
}

Collision::Collision()
{
}

Collision::~Collision()
{
	delete Rect;
	delete OverlapSize;
}

void Collision::DrawDebug(Color color)
{
	SpriteBatch::DrawRectangle(Rect, &color);
}

void Collision::UpdateCollision(Collision &other)
{
	if (other.IsInside(Vector2(Rect->Right(), Rect->Top())))
	{
		cout << "top right" << endl;
		OverlapSize->X += other.Rect->X - (Rect->X + Rect->Width); // Top Right Collision
		OverlapSize->Y += other.Rect->Y - (Rect->Y + Rect->Height); 
	}
	else if(other.IsInside(Vector2(Rect->Left(), Rect->Bottom())))
	{
		cout << "bottom left" << endl;
		OverlapSize->X += (Rect->X + Rect->Width) - other.Rect->X; // Bottom Left Collision
		OverlapSize->Y += (Rect->Y + Rect->Height) - other.Rect->Y;
	}
	else if (other.IsInside(Vector2(Rect->Right(), Rect->Bottom())))
	{
		cout << "bottom right" << endl;
		OverlapSize->X += other.Rect->X - (Rect->X + Rect->Width); // Bottom Right Collision
		OverlapSize->Y += (Rect->Y + Rect->Height) - other.Rect->Y;
	}
	else if (other.IsInside(Vector2(Rect->Left(), Rect->Top())))
	{
		cout << "top left" << endl;
		OverlapSize->X += Rect->X - (other.Rect->X + other.Rect->Width); // Top Left Collision
		OverlapSize->Y += other.Rect->Y - (Rect->Y + Rect->Height); 
	}
}

bool Collision::IsInside(Vector2 point)
{
	return (point.X >= Rect->X && point.X < Rect->X + Rect->Width &&
		point.Y >= Rect->Y && point.Y < Rect->Y + Rect->Height);
}
