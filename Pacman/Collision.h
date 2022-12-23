#include "S2D/S2D.h"
#include <functional>
using namespace S2D;
using namespace std;

#pragma once
class Collision
{
public:
	enum class CollisionType
	{
		Static,
		Dynamic
	};

	function<void()> OnTriggerStay;
	Collision(CollisionType Type);
	Collision();
	~Collision();
	Rect* Rect;
	Vector2* OverlapSize;
	//bool virtual IsColliding(GameObject gameObjectA, GameObject gameObjectB);

	enum class CollisionShape
	{
		None,
		Box,
		Circle,
	};
	
	CollisionShape Shape = CollisionShape::None;
	CollisionType Type = CollisionType::Static;

	void DrawDebug(Color color = Color(1.0f, 1.0f, 1.0f, 0.3f));
	void UpdateCollision(Collision &other);
	bool IsInside(Vector2 point);
	bool IsOverlapping(Collision& other);
};

void lul();