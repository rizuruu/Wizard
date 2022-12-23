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

class GameManager : public Game
{
private:
	// Data to represent Pacman
#pragma region Textures
	Texture2D* T_Player_Idle;
	Texture2D* T_Player_Walk;
	Texture2D* T_Player_Run;
	Texture2D* T_Player_Jump;
	Texture2D* T_Player_Damage;
	Texture2D* T_Player_Dead;
	Texture2D* T_Player_Frozen;
	Texture2D* T_Player_Attack;
	Texture2D* Platform;
	Texture2D* Flower;
	Texture2D* Tile;
	Texture2D* VegetationA;
	Texture2D* WindPlant;
	Texture2D* BGTexture;
	Texture2D* GreenSlime;
	Texture2D* Orb;
	Texture2D* BluePotion;

	Texture2D* T_Monster_Idle;
	Texture2D* T_Monster_Walk;
	Texture2D* T_Monster_Attack;
	Texture2D* T_Monster_Dead;

	Texture2D* T_FreezingBlob_Idle;

	Texture2D* _menuBackground;
	Texture2D* T_PlayButton;
	Texture2D* T_ExitButton;
	Texture2D* T_MenuBG;
#pragma endregion

	Vector2* WizardPrevPosition;
	Rect* WizardRect;
	Rect* Tile_Rect;
	Rect* Tile_Rect2;
	// Data for Menu
	Rect* _menuRectangle;
	Rect* R_HealthBar;
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
	AnimationSequence* A_Player_Idle;
	AnimationSequence* A_Player_Walk;
	AnimationSequence* A_Player_Run;
	AnimationSequence* A_Player_Jump;
	AnimationSequence* A_Player_Attack;
	AnimationSequence* A_Player_Damage;

	AnimationSequence* FlowerAnimator;
	AnimationSequence* WindPlantAnimator;
	AnimationSequence* GreenSlimeAnimator;
	AnimationSequence* OrbAnimator;

	AnimationSequence* A_Monster_Idle;
	AnimationSequence* A_Monster_Walk;
	AnimationSequence* A_Monster_Attack;

	AnimationSequence* A_FreezingBlob_Idle;
#pragma endregion

	const float WitchSpeed = 0.08f;
	const float RunSpeed = 0.8f;

	Vector2* Velocity;
	const float Gravity = 0.15f;
	float JumpForce = 1.0f;
	int MenuSelection = 0;

	CollisionManager* collisionManager;

#pragma region Methods
public:
	/// <summary> Constructs the Pacman class. </summary>
	GameManager(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~GameManager();
	static GameManager* Instance;

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();
	void LoadPlayerTextutres();
	void LoadAITextutres();
	void InitializeSequences();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);
	void virtual MenuUpdate(int elapsedTime);
	void virtual GameUpdate(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);
	void virtual MenuDraw(int elapsedTime);
	void virtual GameDraw(int elapsedTime);

	void DrawPlayerAnimation(int elapsedTime);
	void DrawEnvironmentFront(int elapsedTime);
	void DrawEnvironmentBack(int elapsedTime);

	void virtual InputHandler(int elapsedTime);
	void virtual MenuInput(Input::KeyboardState* KState, Input::MouseState* MState);
	void virtual GameInput(Input::KeyboardState* KState, Input::MouseState* MState);
	void Jump();
	void Sprint();
	void Damage(float damage);
	void SetupCollisions();

	void AudioHanlder();

	void DrawDebugs(bool draw = false);
#pragma endregion

#pragma region Audio & Sound FX
	SoundEffect* BG_Music;
	SoundEffect* SFX_Whoosh;
	SoundEffect* SFX_Hit;
	SoundEffect* SFX_Freeze;
#pragma endregion

#pragma region Collisions
	Collision* PlayerCollider;
	Collision* GroundCollider;
	Collision* EdgePlatformCollider;
	Collision* PlatformCollider;
	Collision* S_PlatformCollider;
#pragma endregion

#pragma region AI Agents
	AIAgent* AI_Monster;
	AIAgent* AI_FreezingBlob;
#pragma endregion

	enum class PlayerState
	{
		Idle,
		Walk,
		Running,
		Jumping,
		Attacking,
		Damage,
		Frozen,
		Stone,
		Dead,
	};	
	
	enum class GameState
	{
		Menu,
		Game,
	};
	
	PlayerState PState = PlayerState::Idle;
	GameState CurrentGameState = GameState::Menu;

	Vector2* WitchPosition;
	float* Health;
};
#endif
void OnStay();