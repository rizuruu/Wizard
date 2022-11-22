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
		std::cout << "yourmum";
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
	cout << Rect->Y;
}

void Collision::UpdateCollision(Collision &other)
{
	if (other.Rect->X + other.Rect->Width >= Rect->X &&
		other.Rect->X + other.Rect->Width < Rect->X + Rect->Width &&
		other.Rect->Y >= Rect->Y &&
		other.Rect->Y < Rect->Y + Rect->Height)
	{
		cout << "top right" << endl;
		OverlapSize->X += other.Rect->X - (Rect->X + Rect->Width); // Top Right Collision
		OverlapSize->Y += other.Rect->Y - (Rect->Y + Rect->Height); // Top Right Collision
	}
	else if (Rect->X + Rect->Width >= other.Rect->X &&
		Rect->X + Rect->Width < other.Rect->X + other.Rect->Width &&
		Rect->Y + Rect->Height >= other.Rect->Y &&
		Rect->Y + Rect->Height < other.Rect->Y + other.Rect->Height)
	{
		cout << "bottom left" << endl;
		OverlapSize->X += Rect->X + ((other.Rect->X + other.Rect->Width) - Rect->Width); // Bottom Left Collision
		OverlapSize->Y += (Rect->Y + Rect->Height) - other.Rect->Y;
	}
	else if (Rect->X >= other.Rect->X &&
		Rect->X < other.Rect->X + other.Rect->Width &&
		Rect->Y + Rect->Height >= other.Rect->Y &&
		Rect->Y + Rect->Height < other.Rect->Y + other.Rect->Height)
	{
		cout << "bottom right" << endl;
		OverlapSize->X += other.Rect->X - (Rect->X + Rect->Width); // Bottom Right Collision
		OverlapSize->Y += Rect->Y - (other.Rect->Y + other.Rect->Height); 

		//OverlapSize = new Vector2(other.Rect->X, other.Rect->Y); // Bottom Right Collision
	}
	else if (other.Rect->X >= Rect->X &&
		other.Rect->X < Rect->X + Rect->Width &&
		other.Rect->Y >= Rect->Y &&
		other.Rect->Y < Rect->Y + Rect->Height)
	{
		cout << "top left" << endl;
		OverlapSize->X += Rect->X - (other.Rect->X + other.Rect->Width); // Top Left Collision
		OverlapSize->Y += other.Rect->Y - (Rect->Y + Rect->Height); 

		//OverlapSize = new Vector2(other.Rect->X + other.Rect->Width, other.Rect->Y + other.Rect->Height); // Top Left Collision
	}
}
