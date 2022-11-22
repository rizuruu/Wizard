#include "Collision.h"
#include <vector>
using namespace std;
#ifndef _cm
#define _cm

#pragma once
class CollisionManager
{
public:
	CollisionManager();

	~CollisionManager();
	static CollisionManager* Instance;
	vector<Collision*>* Collisions;

	void Update(int elapsedTime);
};

#endif // !1