using namespace std;
#include "S2D/S2D.h";
using namespace S2D;

#pragma once
class Sequence
{
public:
	Vector2 GridVector;
	int FramesCount;
	Texture2D SourceTexture;
	Rect* SourceRect;
	Vector2* Position;
};