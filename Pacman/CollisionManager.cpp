#include "CollisionManager.h"
#include <iostream>
CollisionManager* CollisionManager::Instance = NULL;

CollisionManager::CollisionManager()
{
	Instance = this;
	Collisions = new vector<Collision*>();

}

CollisionManager::~CollisionManager()
{
	delete Collisions;
	delete Instance;
}

void CollisionManager::Update(int elapsedTime)
{
	for (int i = 0; i < Collisions->size(); i++)
	{
		if ((*Collisions)[i]->Type == Collision::CollisionType::Dynamic)
		{
			for (int j = 0; j < Collisions->size(); j++)
			{
				if ((*Collisions)[j]->Type == Collision::CollisionType::Static)
				{
					(*Collisions)[i]->UpdateCollision(*(* Collisions)[j]);
				}
				else if (i != j)
				{
					if ((*Collisions)[i]->IsOverlapping(*(*Collisions)[j]))
					{
						(*Collisions)[i]->OnTriggerStay();
						(*Collisions)[j]->OnTriggerStay();
					}
				}
			}
		}
	}
}

bool CollisionManager::IsCollider(Vector2 point)
{
	for (int j = 0; j < Collisions->size(); j++)
	{
		if ((*Collisions)[j]->Type == Collision::CollisionType::Static)
		{
			if (point.X >= (*Collisions)[j]->Rect->X && point.X < (*Collisions)[j]->Rect->X + (*Collisions)[j]->Rect->Width &&
				point.Y >= (*Collisions)[j]->Rect->Y && point.Y < (*Collisions)[j]->Rect->Y + (*Collisions)[j]->Rect->Height)
				return true;
		}
	}

	return false;
}