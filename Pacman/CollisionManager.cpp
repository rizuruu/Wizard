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
			}
		}
	}
}