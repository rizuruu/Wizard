#include "Pacman.h"
#include "AnimationSequence.h"
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>
#include "wtypes.h"
#include <windows.h>

using namespace std;

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.3f), _cPacmanFrameTime(250)
{
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	IdleFramesVector = new S2D::Vector2(20, 1);
	AttackFramesVector = new S2D::Vector2(5, 5);
	FlowerFramesVector = new S2D::Vector2(8, 8);
	CurrentFrame = 0;
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);
	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, desktop.right, desktop.bottom, false, 25, 25, "Pacman", 60);
	Input::Initialise();
	// Start the Game Loop - This calls Update and Draw in game loop
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;
}

void Pacman::LoadContent()
{
	using namespace std::this_thread;     // sleep_for, sleep_until
	using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
	using std::chrono::system_clock;

	IdleAnimator = new AnimationSequence();
	RunAnimator = new AnimationSequence();
	AttackAnimator = new AnimationSequence();
	FlowerAnimator = new AnimationSequence();

	// Load Pacman
	_pacmanTexture = new Texture2D();
	_RunTexture = new Texture2D();
	_AttackTexture = new Texture2D();
	Platform = new Texture2D();
	Flower = new Texture2D();
	_pacmanTexture->Load("Textures/PlayerIdle.png", false);
	Platform->Load("Textures/Platform.png", false);
	Flower->Load("Textures/f.png", false);
	_RunTexture->Load("Textures/PlayerWalk.png", false);
	_AttackTexture->Load("Textures/Ghost2_Attack.png", false);
	_pacmanPosition = new Vector2(-128.0f, (Graphics::GetViewportHeight()/2));
	int t = 0;

	IdleAnimator->Initialize(_pacmanTexture, 20, IdleFramesVector);
	RunAnimator->Initialize(_RunTexture, 20, IdleFramesVector);
	AttackAnimator->Initialize(_AttackTexture, 14, AttackFramesVector);
	FlowerAnimator->Initialize(Flower, 60, FlowerFramesVector, 768);

	_pacmanSourceRect = new Rect(0.0f, 0.0f, 847, 864);

	// Load Munchie
	_munchieBlueTexture = new Texture2D();
	_munchieBlueTexture->Load("Textures/Munchie.tga", true);
	_munchieInvertedTexture = new Texture2D();
	_munchieInvertedTexture->Load("Textures/MunchieInverted.tga", true);
	_munchieRect = new Rect(100.0f, 450.0f, 12, 12);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();

	_menuBackground->Load("Textures/Transparency.png", false);

	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void Pacman::Update(int elapsedTime)
{
	InputHandler(elapsedTime);
}

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y << " frame: " << time;

	SpriteBatch::BeginDraw(); // Starts Drawing
	SpriteBatch::DrawRectangle(new Rect(0, 0, 1920, 1080), new Color(0.0f, 0.341f, 0.416f, 1.0f));
	if (_paused)

	{
		std::stringstream menuStream; menuStream << "PAUSED!";
		_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr); SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	if (!_paused)
		DrawPlayerAnimation(elapsedTime);

	FlowerAnimator->PlaySequence(new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight()/6), false);

	SpriteBatch::Draw(Platform, new Vector2(-20, Graphics::GetViewportHeight() - 124), new Rect(0, 1550, 2048, 498), Vector2::Zero, 0.5f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(Platform, new Vector2(900, Graphics::GetViewportHeight() - 124), new Rect(0, 1550, 2048, 498), Vector2::Zero, 0.5f, 0.0f, Color::White, SpriteEffect::NONE);
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::InputHandler(int elapsedTime)
{
	if (PState == PlayerState::Attacking)
		return;

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();

	if (!_paused)
	{
		if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			//_pacmanPosition->Y -= _cPacmanSpeed * elapsedTime;
			PState = PlayerState::Idle;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
			PState = PlayerState::Running;
			isFlipped = true;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			//_pacmanPosition->Y += _cPacmanSpeed * elapsedTime;
			PState = PlayerState::Idle;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime;
			PState = PlayerState::Running;
			isFlipped = false;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			PState = PlayerState::Attacking;
		}
		else if (mouseState->LeftButton == Input::ButtonState::PRESSED)
			PState = PlayerState::Attacking;
		else
		{
			PState = PlayerState::Idle;
		}

		if (_pacmanPosition->X > Graphics::GetViewportWidth())
		{
			_pacmanPosition->X = -_pacmanSourceRect->Width;
		}
	}

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;

}

void Pacman::DrawPlayerAnimation(int elapsedTime)
{
	Sleep(50);
	CurrentFrame++;

	switch (PState)
	{
		case PlayerState::Idle:
			IdleAnimator->PlaySequence(_pacmanPosition, isFlipped);
			break;
		case PlayerState::Running:
			RunAnimator->PlaySequence(_pacmanPosition, isFlipped);
			break;
		case PlayerState::Attacking:
			if (AttackAnimator->PlaySequenceOnce(_pacmanPosition, isFlipped))
				PState = PlayerState::Idle;
			break;
	}
}