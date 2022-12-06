#pragma once

// If Windows and not in Debug, this will run without a console window
// You can use this to output information when debugging using cout or cerr
#ifdef WIN32 
	#ifndef _DEBUG
		#pragma comment(linker, "/SUBSYSTEM:windows /ENTRY:mainCRTStartup")
	#endif
#endif

// Just need to include main header file
#include "S2D/S2D.h"
#include <list>
#include <vector>
#include "AnimationSequence.h"
#include "Collision.h"
#include "CollisionManager.h"
#include "AIAgent.h"
// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
#ifndef _player
#define _player

class Pacman : public Game
{
private:
	// Data to represent Pacman
#pragma region Textures
	Texture2D* WizardTexture_Idle;
	Texture2D* WizardTexture_Run;
	Texture2D* WizardTexture_Jump;
	Texture2D* _AttackTexture;
	Texture2D* Platform;
	Texture2D* Flower;
	Texture2D* Tile;
	Texture2D* VegetationA;
	Texture2D* WindPlant;
	Texture2D* BGTexture;
	Texture2D* GreenSlime;
	Texture2D* Orb;

	Texture2D* _menuBackground;
#pragma endregion

	Vector2* WizardPrevPosition;
	Rect* WizardRect;
	Rect* Tile_Rect;
	Rect* Tile_Rect2;
	// Data for Menu
	Rect* _menuRectangle;
	// Position for String

	Vector2* _stringPosition;
	Vector2* _menuStringPosition; 

	bool _paused;
	bool _pKeyDown;
	bool isFlipped;
	bool _drawDebug;

#pragma region Sprite Grid Vectors
	Vector2 IdleFramesVector;
	Vector2 RunFramesVector;
	Vector2 JumpFramesVector;
	Vector2 AttackFramesVector;
	Vector2 FlowerFramesVector;
#pragma endregion

#pragma region Animation Sequences
	AnimationSequence* IdleAnimator;
	AnimationSequence* RunAnimator;
	AnimationSequence* JumpAnimator;
	AnimationSequence* AttackAnimator;
	AnimationSequence* FlowerAnimator;
	AnimationSequence* WindPlantAnimator;
	AnimationSequence* GreenSlimeAnimator;
	AnimationSequence* OrbAnimator;
#pragma endregion

	SoundEffect* music;

	const float WizardSpeed = 0.1f;

	Vector2* Velocity;
	const float Gravity = 0.15f;
	float JumpForce = 1.0f;

	CollisionManager* collisionManager;
public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();
	static Pacman* Instance;

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	void virtual InitializeSequences();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void virtual DrawPlayerAnimation(int elapsedTime);
	void virtual DrawEnvironmentFront(int elapsedTime);
	void virtual DrawEnvironmentBack(int elapsedTime);
	void virtual InputHandler(int elapsedTime);
	void virtual Jump(int elapsedTime);
	void virtual SetupCollisions();
	int random(int min, int max);

	void virtual AudioHanlder();

	void virtual DrawDebugs(bool draw = false);

#pragma region Collisions
	Collision* gameObjectA;
	Collision* gameObjectB;
	Collision* gameObjectC;
	Collision* PlatformCollider;
#pragma endregion

#pragma region AI Agents
	AIAgent* Enemy;
#pragma endregion

	enum class PlayerState
	{
		Idle,
		Running,
		Jumping,
		Attacking,
	};
	
	PlayerState PState = PlayerState::Idle;

	Vector2* WizardPosition;
};
#endif