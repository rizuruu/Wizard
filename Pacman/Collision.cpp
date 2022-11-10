#include "Collision.h"

//bool Collision::IsColliding(GameObject gameObjectA, GameObject gameObjectB)
//{
//	switch (gameObjectA.Collision.Type)
//	{
//		case CollisionType::None:
//			//Do nothing
//			break;
//		case CollisionType::Box:
//			if (gameObjectA.Position.X >= gameObjectB.Position.X &&
//				gameObjectA.Position.X < gameObjectB.Position.X + gameObjectB.Collision.Width &&
//				gameObjectA.Position.Y >= gameObjectB.Position.Y &&
//				gameObjectA.Position.Y < gameObjectB.Position.Y + gameObjectB.Collision.Height)
//				return true; // Bottom Right Collision
//			else if (gameObjectB.Position.X >= gameObjectA.Position.X &&
//				gameObjectB.Position.X < gameObjectA.Position.X + gameObjectA.Collision.Width &&
//				gameObjectB.Position.Y >= gameObjectA.Position.Y &&
//				gameObjectB.Position.Y < gameObjectA.Position.Y + gameObjectA.Collision.Height)
//				return true; // Top Left Collision
//			else if (gameObjectB.Position.X + gameObjectB.Collision.Width >= gameObjectA.Position.X &&
//				gameObjectB.Position.X + gameObjectB.Collision.Width < gameObjectA.Position.X + gameObjectA.Collision.Width &&
//				gameObjectB.Position.Y + gameObjectB.Collision.Height >= gameObjectA.Position.Y &&
//				gameObjectB.Position.Y + gameObjectB.Collision.Height < gameObjectA.Position.Y + gameObjectA.Collision.Height)
//				return true; // Top Right Collision
//			else if (gameObjectA.Position.X + gameObjectA.Collision.Width >= gameObjectB.Position.X &&
//				gameObjectA.Position.X + gameObjectA.Collision.Width < gameObjectB.Position.X + gameObjectB.Collision.Width &&
//				gameObjectA.Position.Y + gameObjectA.Collision.Height >= gameObjectB.Position.Y &&
//				gameObjectA.Position.Y + gameObjectA.Collision.Height < gameObjectB.Position.Y + gameObjectB.Collision.Height)
//				return true; // Bottom Left Collision
//			else 
//				return false;
//			break;
//		case CollisionType::Circle:
//			break;
//		default:
//			return false;
//	}
//}
