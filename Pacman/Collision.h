#include "S2D/S2D.h"

using namespace S2D;

#pragma once
class Collision
{
public:
	int Radius;
	int Width;
	int Height;
	float X;
	float Y;

	//bool virtual IsColliding(GameObject gameObjectA, GameObject gameObjectB);

	enum class CollisionType
	{
		None,
		Box,
		Circle,
	};

	CollisionType Type = CollisionType::None;
};