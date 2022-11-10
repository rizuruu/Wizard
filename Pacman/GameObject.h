#include <string>
#include "S2D/S2D.h"
#include "Collision.h"
using namespace S2D;
using namespace std;

#pragma once
class GameObject
{
public:
	GameObject();

	string Name;
	Collision Collision;
	Vector2 Position;

	bool virtual IsColliding(GameObject gameObject);
};

