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

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), WizardSpeed(0.3f)
{
	Pacman::Instance = this;
	collisionManager = new CollisionManager();
	SetupCollisions();

	_paused = false;
	_pKeyDown = false;
	_drawDebug = false;
	IdleFramesVector = Vector2(20, 1);
	JumpFramesVector = Vector2(3, 2);
	AttackFramesVector = Vector2(5, 5);
	FlowerFramesVector = Vector2(10, 6);

	Enemy = new AIAgent();

	RECT desktop;
	const HWND hDesktop = GetDesktopWindow();
	GetWindowRect(hDesktop, &desktop);

	//Initialise important Game aspects
	S2D::Audio::Initialise();
	Graphics::Initialise(argc, argv, this, desktop.right, desktop.bottom, false, 25, 25, "Wizard", 60);
	Input::Initialise();
	Graphics::StartGameLoop();
}

Pacman::~Pacman()
{
	delete WizardTexture_Idle;
	delete WizardRect;
	delete WizardPosition;
	delete Instance;

	delete PlayerCollider;
	delete GroundCollider;
	delete EdgePlatformCollider;
	delete PlatformCollider;

	delete Enemy;
	delete Velocity;
	delete BG_Music;
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
	WizardTexture_Idle = new Texture2D();
	WizardTexture_Run = new Texture2D();
	WizardTexture_Jump = new Texture2D();
	_AttackTexture = new Texture2D();
	Platform = new Texture2D();
	Flower = new Texture2D();
	Tile = new Texture2D();
	VegetationA = new Texture2D();
	WindPlant = new Texture2D();
	BGTexture = new Texture2D();
	GreenSlime = new Texture2D();
	Orb = new Texture2D();
	BluePotion = new Texture2D();
	WizardTexture_Idle->Load("Textures/PlayerIdle.png", false);
	Platform->Load("Textures/Platform.png", false);
	Flower->Load("Textures/Flower.png", false);
	WindPlant->Load("Textures/Plants/WindPlant.png", false);
	WizardTexture_Run->Load("Textures/PlayerWalk.png", false);
	WizardTexture_Jump->Load("Textures/Jump.png", false);
	_AttackTexture->Load("Textures/Ghost2_Attack.png", false);
	Tile->Load("Textures/TileSet.png", false);
	VegetationA->Load("Textures/VegetationA.png", false);
	BGTexture->Load("Textures/BG.png", false);
	GreenSlime->Load("Textures/GreenSlime.png", false);
	Orb->Load("Textures/Orb.png", false);
	BluePotion->Load("Textures/BluePotion.png", false);
	WizardPosition = new Vector2(-128.0f, Graphics::GetViewportHeight()/3);
	WizardPrevPosition = new Vector2(-128.0f, Graphics::GetViewportHeight()/3);
	Velocity = new Vector2(0.0f, 0.0f);
	int t = 0;

	InitializeSequences();

	WizardRect = new Rect(0.0f, 0.0f, 847, 864);
	Tile_Rect = new Rect(512.0f, 0.0f, 512, 512);
	Tile_Rect2 = new Rect(512.0f, 1024.0f, 512, 512);

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
	sequence.Source = WizardTexture_Idle;
	sequence.width = 512;
	sequence.height = 512;
	IdleAnimator->Initialize(sequence);

	RunAnimator->Initialize(WizardTexture_Run, 20, IdleFramesVector, 512, 512);
	AttackAnimator->Initialize(_AttackTexture, 14, AttackFramesVector, 512, 512);
	JumpAnimator->Initialize(WizardTexture_Jump, 6, JumpFramesVector, 512, 512);

	FlowerAnimator->Initialize(Flower, 60, FlowerFramesVector, 768, 768);
	WindPlantAnimator->Initialize(WindPlant, 30, Vector2(6, 5), 512, 512);
	GreenSlimeAnimator->Initialize(GreenSlime, 30, Vector2(5, 6), 376, 256);
	OrbAnimator->Initialize(Orb, 30, Vector2(5, 6), 480, 480);
}

void Pacman::Update(int elapsedTime)
{
	InputHandler(elapsedTime);
	WizardPrevPosition->X = WizardPosition->X;
	WizardPrevPosition->Y = WizardPosition->Y;
	WizardPosition->X += Velocity->X * elapsedTime;

	WizardPosition->Y += Velocity->Y * elapsedTime;

	PlayerCollider->Rect->X = WizardPosition->X + 150;
	PlayerCollider->Rect->Y = WizardPosition->Y + 130;

	GroundCollider->Rect->X = 0;
	GroundCollider->Rect->Y = Graphics::GetViewportHeight() - 73;

	EdgePlatformCollider->Rect->X = Graphics::GetViewportWidth() - (5 * 73 - 20);
	EdgePlatformCollider->Rect->Y = (Graphics::GetViewportHeight() - 200);

	PlatformCollider->Rect->X = (Graphics::GetViewportWidth() / 2) - (73 * 2);
	PlatformCollider->Rect->Y = (Graphics::GetViewportHeight() / 2);

	collisionManager->Update(elapsedTime);
	Enemy->UpdateAI(elapsedTime);

	if (abs(PlayerCollider->OverlapSize->X) > 0.0f || abs(PlayerCollider->OverlapSize->Y) > 0.0f)
	{
		cout << abs(PlayerCollider->OverlapSize->Y) << endl;
		if (abs(PlayerCollider->OverlapSize->X) >= PlayerCollider->Rect->Width)
		{
			while (collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height)) ||
				collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X + PlayerCollider->Rect->Width, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height)))
			{
				WizardPosition->Y--;
				Velocity->Y = 0;
				PlayerCollider->Rect->X = WizardPosition->X + 150;
				PlayerCollider->Rect->Y = WizardPosition->Y + 130;
			}

		}

		while (collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X, PlayerCollider->Rect->Y + 1.0f)) ||
			collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X + PlayerCollider->Rect->Width, PlayerCollider->Rect->Y + 1.0f)))
		{
			WizardPosition->Y++;
			Velocity->Y = 0;
			PlayerCollider->Rect->X = WizardPosition->X + 150;
			PlayerCollider->Rect->Y = WizardPosition->Y + 130;
		}

		WizardPosition->X = WizardPrevPosition->X;
		PlayerCollider->OverlapSize->X = 0;

		PlayerCollider->OverlapSize->Y = 0;
		
		if (Velocity->X == 0)
			PState = PlayerState::Idle;

	}

	if (!collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height + 1.0f)) &&
		!collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X + PlayerCollider->Rect->Width, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height + 1.0f)))
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
	stream << "Player X: " << WizardPosition->X << " Y: " << WizardPosition->Y << " Velocity X: " << Velocity->X << " Velocity Y: " << Velocity->Y << " Enemy X: " << Enemy->velocity->X << " Y: " << Enemy->velocity->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing

	switch (CurrentGameState)
	{
	case Pacman::GameState::Menu:
		break;
	case Pacman::GameState::Game:
		break;
	default:
		break;
	}

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
	DrawDebugs(_drawDebug);

	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::DrawRectangle(new Rect(25, 30, 250, 20), new Color(0.0f, 1.f, 0.0f, 0.8f)); //0.443, 0.761, 0.788

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
			PState = PlayerState::Idle;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			Velocity->X = -WizardSpeed;
			PState = PlayerState::Running;
			isFlipped = true;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			PState = PlayerState::Idle;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			Velocity->X = WizardSpeed;
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

		if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
		{
			Dash();
		}

		if (WizardPosition->X > Graphics::GetViewportWidth())
		{
			WizardPosition->X = -WizardRect->Width;
		}
	}

	if (keyboardState->IsKeyDown(Input::Keys::P) && !_pKeyDown)
	{
		_pKeyDown = true;
		_paused = !_paused;
	}
	if (keyboardState->IsKeyUp(Input::Keys::P))
		_pKeyDown = false;

	if (keyboardState->IsKeyDown(Input::Keys::DECIMAL))
		_drawDebug = !_drawDebug;
}

void Pacman::DrawPlayerAnimation(int elapsedTime)
{
	Sleep(40);

	switch (PState)
	{
		case PlayerState::Idle:
			IdleAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Running:
			RunAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Jumping:
			JumpAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Attacking:
			if (AttackAnimator->PlaySequenceOnce(WizardPosition, isFlipped, 0.8f))
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
	
	SpriteBatch::Draw(BluePotion, new Vector2((Graphics::GetViewportWidth() / 2), (Graphics::GetViewportHeight() - 150)), new Rect(128.0f, 128.0f, 128.0f, 128.0f), Vector2::Zero, 0.5f, 0.0f, Color::White, SpriteEffect::NONE);
	
	//OrbAnimator->PlaySequence(new Vector2(Graphics::GetViewportWidth() / 2.0f - 200, (Graphics::GetViewportHeight() - 280)), false, 0.3f);
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
	if (collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height + 1.0f)) ||
		collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X + PlayerCollider->Rect->Width, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height + 1.0f)))
		Velocity->Y = -JumpForce;
}

void Pacman::Dash()
{
	if (isFlipped)
		Velocity->X = -MathHelper::Lerp(0.0f, 2.5f, 0.4);
	else
		Velocity->X = MathHelper::Lerp(0.0f, 2.5f, 0.4);
}

void Pacman::SetupCollisions()
{
	PlayerCollider = new Collision(Collision::CollisionType::Dynamic);
	PlayerCollider->Rect->Width = 100.0f;
	PlayerCollider->Rect->Height = 170.0f;
	PlayerCollider->Rect->X = 1920 / 2;
	PlayerCollider->Rect->Y = 1080 / 2;


	GroundCollider = new Collision(Collision::CollisionType::Static);
	GroundCollider->Rect->Width = 1920;
	GroundCollider->Rect->Height = 1000;

	EdgePlatformCollider = new Collision(Collision::CollisionType::Static);
	EdgePlatformCollider->Rect->Width = 73 * 5;
	EdgePlatformCollider->Rect->Height = 73 * 2;
	EdgePlatformCollider->Rect->X = 1600;
	EdgePlatformCollider->Rect->Y = 0;

	PlatformCollider = new Collision(Collision::CollisionType::Static);
	PlatformCollider->Rect->Height = 73 * 2;
	PlatformCollider->Rect->Width = 73 * 4;
}

void Pacman::DrawDebugs(bool draw)
{
	if (draw)
	{
		PlayerCollider->DrawDebug();
		GroundCollider->DrawDebug(Color(1.0f, 0.0f, 0.0f, 0.3f));
		EdgePlatformCollider->DrawDebug(Color(0.0f, 1.0f, 0.0f, 0.3f));
		PlatformCollider->DrawDebug(Color(0.0f, 1.0f, 0.0f, 0.3f));
		Enemy->collision->DrawDebug(Color(1.0f, 0.0f, 0.0f, 0.3f));
	}
}

int Pacman::random(int min, int max) 
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void Pacman::AudioHanlder()
{
	BG_Music = new SoundEffect(true, 1.0f, 0.2f);
	BG_Music->Load("Audio/BG_Music.wav");
	Audio::Play(BG_Music);
}