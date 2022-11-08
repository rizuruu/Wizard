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
// Reduces the amount of typing by including all classes in S2D namespace
using namespace S2D;

// Declares the Pacman class which inherits from the Game class.
// This allows us to overload the Game class methods to help us
// load content, draw and update our game.
class Pacman : public Game
{
private:
	// Data to represent Pacman
	Vector2* _pacmanPosition;
	Rect* _pacmanSourceRect;
	Texture2D* _pacmanTexture;
	Texture2D* _RunTexture;
	Texture2D* _AttackTexture;
	Rect* PlayerIdleFrames[24];
	Rect* PlayerRunFrames[22];
	Rect* PlayerAttackFrames[21];
	
	// Data to represent Munchie
	int _frameCount;
	Rect* _munchieRect;
	Texture2D* _munchieBlueTexture;
	Texture2D* _munchieInvertedTexture;

	// Position for String
	Vector2* _stringPosition;

	// Data for Menu
	Texture2D* _menuBackground; 
	Rect* _menuRectangle; 
	Vector2* _menuStringPosition; 
	bool _paused;
	bool _pKeyDown;

	int _pacmanFrame;
	int _pacmanCurrentFrameTime;
	const int _cPacmanFrameTime;

	Vector2* IdleFramesVector;
	Vector2 RunFramesVector;
	Vector2* AttackFramesVector;
	int CurrentFrame;

	AnimationSequence* IdleAnimator;
	AnimationSequence* RunAnimator;
	AnimationSequence* AttackAnimator;

public:
	/// <summary> Constructs the Pacman class. </summary>
	Pacman(int argc, char* argv[]);

	/// <summary> Destroys any data associated with Pacman class. </summary>
	virtual ~Pacman();

	/// <summary> All content should be loaded in this method. </summary>
	void virtual LoadContent();

	/// <summary> Called every frame - update game logic here. </summary>
	void virtual Update(int elapsedTime);

	/// <summary> Called every frame - draw game here. </summary>
	void virtual Draw(int elapsedTime);

	void virtual DrawPlayerAnimation(int elapsedTime);
	void virtual InputHandler(int elapsedTime);

	const float _cPacmanSpeed = 0.1f;

	enum class PlayerState
	{
		Idle,
		Running,
		Attacking,
	};
	
	PlayerState PState = PlayerState::Idle;
};