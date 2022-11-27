#include "Pacman.h"
#include "AnimationSequence.h"
#include <vector>
#include <chrono>
#include <thread>
#include <sstream>
#include "wtypes.h"
#include <windows.h>
#include <iostream>
#include <cstdlib>
#include "Collision.h"
using namespace std;
Pacman* Pacman::Instance = NULL;

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.3f), _cPacmanFrameTime(250)
{
	Pacman::Instance = this;
	collisionManager = new CollisionManager();

	hasJumped = false;
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;
	_pacmanCurrentFrameTime = 0;
	_pacmanFrame = 0;
	IdleFramesVector = new S2D::Vector2(20, 1);
	JumpFramesVector = new S2D::Vector2(3, 2);
	AttackFramesVector = new S2D::Vector2(5, 5);
	FlowerFramesVector = new S2D::Vector2(10, 6);

	gameObjectA = new Collision(Collision::CollisionType::Dynamic);
	gameObjectA->Rect->Width = 100.0f;
	gameObjectA->Rect->Height = 170.0f;
	gameObjectA->Rect->X = 1920/2;
	gameObjectA->Rect->Y = 1080/2;
	

	gameObjectB = new Collision(Collision::CollisionType::Static);
	gameObjectB->Rect->Width = 1920;
	gameObjectB->Rect->Height = 1000;

	gameObjectC = new Collision(Collision::CollisionType::Static);
	gameObjectC->Rect->Width = 73*5;
	gameObjectC->Rect->Height = 73*2;
	gameObjectC->Rect->X = 1600;
	gameObjectC->Rect->Y = 0;

	PlatformCollider = new Collision(Collision::CollisionType::Static);
	PlatformCollider->Rect->Height = 73 * 2;
	PlatformCollider->Rect->Width = 73 * 4;

	Enemy = new AIAgent();

	CurrentFrame = 0;
	Acc = 0;
	RECT desktop;
	// Get a handle to the desktop window
	const HWND hDesktop = GetDesktopWindow();
	// Get the size of screen to the variable desktop
	GetWindowRect(hDesktop, &desktop);

	//Initialise important Game aspects
	S2D::Audio::Initialise();
	Graphics::Initialise(argc, argv, this, desktop.right, desktop.bottom, false, 25, 25, "Wizard", 60);
	Input::Initialise();
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete _pacmanTexture;
	delete _pacmanSourceRect;
	delete _munchieBlueTexture;
	delete _munchieInvertedTexture;
	delete _munchieRect;

	delete gameObjectA;
	delete gameObjectB;
	delete gameObjectC;
	delete PlatformCollider;
}

void Pacman::LoadContent()
{
	IdleAnimator = new AnimationSequence();
	RunAnimator = new AnimationSequence();
	JumpAnimator = new AnimationSequence();
	AttackAnimator = new AnimationSequence();
	FlowerAnimator = new AnimationSequence();
	WindPlantAnimator = new AnimationSequence();
	GreenSlimeAnimator = new AnimationSequence();
	OrbAnimator = new AnimationSequence();
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_RunTexture = new Texture2D();
	_JumpTexture = new Texture2D();
	_AttackTexture = new Texture2D();
	Platform = new Texture2D();
	Flower = new Texture2D();
	Tile = new Texture2D();
	VegetationA = new Texture2D();
	WindPlant = new Texture2D();
	BGTexture = new Texture2D();
	GreenSlime = new Texture2D();
	Orb = new Texture2D();
	_pacmanTexture->Load("Textures/PlayerIdle.png", false);
	Platform->Load("Textures/Platform.png", false);
	Flower->Load("Textures/Flower.png", false);
	WindPlant->Load("Textures/Plants/WindPlant.png", false);
	_RunTexture->Load("Textures/PlayerWalk.png", false);
	_JumpTexture->Load("Textures/Jump.png", false);
	_AttackTexture->Load("Textures/Ghost2_Attack.png", false);
	Tile->Load("Textures/TileSet.png", false);
	VegetationA->Load("Textures/VegetationA.png", false);
	BGTexture->Load("Textures/BG.png", false);
	GreenSlime->Load("Textures/GreenSlime.png", false);
	Orb->Load("Textures/Orb.png", false);
	_pacmanPosition = new Vector2(-128.0f, Graphics::GetViewportHeight()/3);
	_pacmanPrevPosition = new Vector2(-128.0f, Graphics::GetViewportHeight()/3);
	_JumpPosition = new Vector2(_pacmanPosition->X, _pacmanPosition->Y - 80);
	Velocity = new Vector2(0.0f, 0.0f);
	int t = 0;

	InitializeSequences();

	_pacmanSourceRect = new Rect(0.0f, 0.0f, 847, 864);
	Tile_Rect = new Rect(512.0f, 0.0f, 512, 512);
	Tile_Rect2 = new Rect(512.0f, 1024.0f, 512, 512);

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

void Pacman::InitializeSequences()
{
	Sequence sequence;
	sequence.FramesCount = 20;
	sequence.grid = IdleFramesVector;
	sequence.Source = _pacmanTexture;
	sequence.width = 512;
	sequence.height = 512;
	IdleAnimator->Initialize(sequence);

	RunAnimator->Initialize(_RunTexture, 20, IdleFramesVector, 512, 512);
	AttackAnimator->Initialize(_AttackTexture, 14, AttackFramesVector, 512, 512);
	JumpAnimator->Initialize(_JumpTexture, 6, JumpFramesVector, 512, 512);

	FlowerAnimator->Initialize(Flower, 60, FlowerFramesVector, 768, 768);
	WindPlantAnimator->Initialize(WindPlant, 30, new Vector2(6, 5), 512, 512);
	GreenSlimeAnimator->Initialize(GreenSlime, 30, new Vector2(5, 6), 376, 256);
	OrbAnimator->Initialize(Orb, 30, new Vector2(5, 6), 480, 480);
}

void Pacman::Update(int elapsedTime)
{
	InputHandler(elapsedTime);
	_pacmanPrevPosition->X = _pacmanPosition->X;
	_pacmanPrevPosition->Y = _pacmanPosition->Y;
	_pacmanPosition->X += Velocity->X * elapsedTime;

	_pacmanPosition->Y += Velocity->Y * elapsedTime;

	gameObjectA->Rect->X = _pacmanPosition->X + 150;
	gameObjectA->Rect->Y = _pacmanPosition->Y + 130;

	gameObjectB->Rect->X = 0;
	gameObjectB->Rect->Y = Graphics::GetViewportHeight() - 73;

	gameObjectC->Rect->X = Graphics::GetViewportWidth() - (5 * 73 - 20);
	gameObjectC->Rect->Y = (Graphics::GetViewportHeight() - 200);

	PlatformCollider->Rect->X = (Graphics::GetViewportWidth() / 2) - (73 * 2);
	PlatformCollider->Rect->Y = (Graphics::GetViewportHeight() / 2);

	collisionManager->Update(elapsedTime);
	Enemy->UpdateAI(elapsedTime);

	if (abs(gameObjectA->OverlapSize->X) > 0.0f || abs(gameObjectA->OverlapSize->Y) > 0.0f)
	{
		cout << abs(gameObjectA->OverlapSize->Y) << endl;
		if (abs(gameObjectA->OverlapSize->X) >= gameObjectA->Rect->Width)
		{
			while (collisionManager->IsCollider(Vector2(gameObjectA->Rect->X, gameObjectA->Rect->Y + gameObjectA->Rect->Height)) ||
				collisionManager->IsCollider(Vector2(gameObjectA->Rect->X + gameObjectA->Rect->Width, gameObjectA->Rect->Y + gameObjectA->Rect->Height)))
			{
				_pacmanPosition->Y--;
				Velocity->Y = 0;
				gameObjectA->Rect->X = _pacmanPosition->X + 150;
				gameObjectA->Rect->Y = _pacmanPosition->Y + 130;
			}

		}

		while (collisionManager->IsCollider(Vector2(gameObjectA->Rect->X, gameObjectA->Rect->Y + 1.0f)) ||
			collisionManager->IsCollider(Vector2(gameObjectA->Rect->X + gameObjectA->Rect->Width, gameObjectA->Rect->Y + 1.0f)))
		{
			_pacmanPosition->Y++;
			Velocity->Y = 0;
			gameObjectA->Rect->X = _pacmanPosition->X + 150;
			gameObjectA->Rect->Y = _pacmanPosition->Y + 130;
		}

		_pacmanPosition->X = _pacmanPrevPosition->X;
		gameObjectA->OverlapSize->X = 0;

		gameObjectA->OverlapSize->Y = 0;
		
		if (Velocity->X == 0)
			PState = PlayerState::Idle;

	}

	if (!collisionManager->IsCollider(Vector2(gameObjectA->Rect->X, gameObjectA->Rect->Y + gameObjectA->Rect->Height + 1.0f)) &&
		!collisionManager->IsCollider(Vector2(gameObjectA->Rect->X + gameObjectA->Rect->Width, gameObjectA->Rect->Y + gameObjectA->Rect->Height + 1.0f)))
	{
		PState = PlayerState::Jumping;
		Velocity->Y += Gravity;
	}
	else
		Velocity->Y = 0;
}

void Pacman::Draw(int elapsedTime)
{
	std::stringstream stream;
	stream << "Player X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y << " Velocity X: " << Velocity->X << " Velocity Y: " << Velocity->Y << " Enemy X: " << Enemy->velocity->X << " Y: " << Enemy->velocity->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing
	DrawEnvironmentBack(elapsedTime);
	SpriteBatch::DrawRectangle(new Rect(0, 0, 1920, 1080), new Color(0.0f, 0.341f, 0.416f, 0.5f)); //0.443, 0.761, 0.788

	if (_paused)
	{
		std::stringstream menuStream; menuStream << "PAUSED!";
		_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr); SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}
	else
	{
		DrawPlayerAnimation(elapsedTime);
	}

	Enemy->DrawAI(GreenSlimeAnimator);
	DrawEnvironmentFront(elapsedTime);
	DrawDebugs(false);

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}

void Pacman::InputHandler(int elapsedTime)
{
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
			//_pacmanPosition->X -= _cPacmanSpeed * elapsedTime;
			Velocity->X = -_cPacmanSpeed;
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
			Velocity->X = _cPacmanSpeed;
			PState = PlayerState::Running;
			isFlipped = false;
		}
		else
		{
			PState = PlayerState::Idle;
			Velocity->X = 0.0f;
		}

		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			Jump(elapsedTime);
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
			IdleAnimator->PlaySequence(_pacmanPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Running:
			RunAnimator->PlaySequence(_pacmanPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Jumping:
			JumpAnimator->PlaySequence(_pacmanPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Attacking:
			if (AttackAnimator->PlaySequenceOnce(_pacmanPosition, isFlipped, 0.8f))
				PState = PlayerState::Idle;
			break;
	}
}

void Pacman::DrawEnvironmentFront(int elapsedTime)
{
	// Bottom right 
	for (int i = 1; i < 6; i++)
	{
		int r = random(1, 2);
		SpriteBatch::Draw(Tile, new Vector2(Graphics::GetViewportWidth() - (i * 73), (Graphics::GetViewportHeight() - 200)), new Rect(0.0f, 0.0f, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	// Ground
	for (int i = 0; i < 26; i++)
	{
		SpriteBatch::Draw(Tile, new Vector2(0 + i * 73, Graphics::GetViewportHeight() - 73), Tile_Rect, Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	// Top
	for (int i = 2; i < 26; i++)
	{
		SpriteBatch::Draw(Tile, new Vector2(73 * i, -73), Tile_Rect2, Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	for (int i = 0; i < 3; i++)
	{
		SpriteBatch::Draw(Tile, new Vector2(73 * i, -73), new Rect(1024.0f, 512.0f, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	for (int i = 0; i < 3; i++)
	{
		SpriteBatch::Draw(Tile, new Vector2(73 * i, 73), new Rect(1024.0f, 1024.0f, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	}

	// Platform
	for (int i = 0; i < 3; i++)
	{
	}
	SpriteBatch::Draw(Tile, new Vector2((Graphics::GetViewportWidth() / 2) - 73 * 3, (Graphics::GetViewportHeight() / 2)), new Rect(0 * 512.0f, 3 * 512.0f, 512, 510), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(Tile, new Vector2((Graphics::GetViewportWidth() / 2) - 73, (Graphics::GetViewportHeight() / 2)), new Rect(1 * 512.0f, 3 * 512.0f, 512, 510), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(Tile, new Vector2((Graphics::GetViewportWidth() / 2) + 73, (Graphics::GetViewportHeight() / 2)), new Rect(2 * 512.0f, 3 * 512.0f, 512, 510), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	
	OrbAnimator->PlaySequence(new Vector2(Graphics::GetViewportWidth() / 2.0f - 200, (Graphics::GetViewportHeight() - 280)), false, 0.3f);

	//SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) + 73, (Graphics::GetViewportHeight() / 2)), new Rect(512.0f * 4, 512.0f * 0, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	//SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) + 73, (Graphics::GetViewportHeight() / 2)), new Rect(512.0f * 5, 512.0f * 0, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
}

void Pacman::DrawEnvironmentBack(int elapsedTime)
{
	SpriteBatch::Draw(BGTexture, new Vector2((Graphics::GetViewportWidth() / 2), (Graphics::GetViewportHeight() / 2)), new Rect(2148.0, 2350.0, 1950.0f, 2048.0f), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);

	FlowerAnimator->PlaySequence(new Vector2(Graphics::GetViewportWidth() / 2.0f, (Graphics::GetViewportHeight() - 200)), false, 0.3f);
	WindPlantAnimator->PlaySequence(new Vector2(Graphics::GetViewportWidth() / 2.0f - 200, (Graphics::GetViewportHeight() - 180)), false, 0.3f);
	for (int i = 0; i < 3; i++)
	{
		SpriteBatch::Draw(Tile, new Vector2(73 * i, Graphics::GetViewportHeight() - 300), new Rect(0.0f, 0.0f, 510, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::FLIPHORIZONTAL);
	}
	SpriteBatch::Draw(Tile, new Vector2(73 * 2, (Graphics::GetViewportHeight() - 200)), new Rect(1024.0f, 512.0f, 510, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(VegetationA, new Vector2(73 * 2, (Graphics::GetViewportHeight() - 200)), new Rect(0.0f, 7 * 1024.0f, 510, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);

	// rock
	SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) - (73 * 3 - 30), (Graphics::GetViewportHeight() / 2) - 73 * 3), new Rect(512.0f * (3 + 0), 512.0f * 0, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) - (73 - 30), (Graphics::GetViewportHeight() / 2) - 73 * 3), new Rect(512.0f * (3 + 1), 512.0f * 0, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);

	SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) - (73 * 3 - 30), (Graphics::GetViewportHeight() / 2) - 73 * 1), new Rect(512.0f * (3 + 0), 512.0f * 1, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) - (73 - 30), (Graphics::GetViewportHeight() / 2) - 73 * 1), new Rect(512.0f * (3 + 1), 512.0f * 1, 512, 512), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
	SpriteBatch::Draw(VegetationA, new Vector2((Graphics::GetViewportWidth() / 2) + (73 + 30), (Graphics::GetViewportHeight() / 2) - 73 * 1), new Rect(512.0f * 5, 512.0f * 1, 256, 450), Vector2::Zero, 0.3f, 0.0f, Color::White, SpriteEffect::NONE);
}

void Pacman::Jump(int elapsedTime)
{
	if (collisionManager->IsCollider(Vector2(gameObjectA->Rect->X, gameObjectA->Rect->Y + gameObjectA->Rect->Height + 1.0f)) ||
		collisionManager->IsCollider(Vector2(gameObjectA->Rect->X + gameObjectA->Rect->Width, gameObjectA->Rect->Y + gameObjectA->Rect->Height + 1.0f)))
		Velocity->Y = -JumpForce;
	//if (Enemy->rect->Y == Enemy->PrevPosition->Y)
	//	Enemy->velocity->Y = -JumpForce;
}

void Pacman::DrawDebugs(bool draw)
{
	if (draw)
	{
		gameObjectA->DrawDebug();
		gameObjectB->DrawDebug(Color(1.0f, 0.0f, 0.0f, 0.3f));
		gameObjectC->DrawDebug(Color(0.0f, 1.0f, 0.0f, 0.3f));
		PlatformCollider->DrawDebug(Color(0.0f, 1.0f, 0.0f, 0.3f));
		Enemy->collision->DrawDebug(Color(1.0f, 0.0f, 0.0f, 0.3f));
	}
}

int Pacman::random(int min, int max) //range : [min, max]
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL)); //seeding for the first time only!
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void Pacman::AudioHanlder()
{
	music = new SoundEffect(true, 1.0f, 0.2f);
	music->Load("Audio/BG_Music.wav");
	Audio::Play(music);
}