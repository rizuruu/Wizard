#include "GameManager.h"
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
GameManager* GameManager::Instance = NULL;

GameManager::GameManager(int argc, char* argv[]) : Game(argc, argv), WizardSpeed(0.3f)
{
	GameManager::Instance = this;
	collisionManager = new CollisionManager();
	SetupCollisions();

	_paused = false;
	_pKeyDown = false;
	_drawDebug = false;
	IdleFramesVector = Vector2(5, 3);
	JumpFramesVector = Vector2(3, 2);
	AttackFramesVector = Vector2(5, 4);
	FlowerFramesVector = Vector2(10, 6);
	Health = new float(1.0f);
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

GameManager::~GameManager()
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

void GameManager::LoadContent()
{
	IdleAnimator = new AnimationSequence();
	WalkAnimator = new AnimationSequence();
	RunAnimator = new AnimationSequence();
	JumpAnimator = new AnimationSequence();
	AttackAnimator = new AnimationSequence();
	DamageAnimator = new AnimationSequence();
	FlowerAnimator = new AnimationSequence();
	WindPlantAnimator = new AnimationSequence();
	GreenSlimeAnimator = new AnimationSequence();
	OrbAnimator = new AnimationSequence();
	MonsterIdleAnimator = new AnimationSequence();
	MonsterRunAnimator = new AnimationSequence();
	MonsterAttackAnimator = new AnimationSequence();

	// Load Pacman
	WizardTexture_Idle = new Texture2D();
	WizardTexture_Walk = new Texture2D();
	WizardTexture_Run = new Texture2D();
	WizardTexture_Jump = new Texture2D();
	_AttackTexture = new Texture2D();
	WizardTexture_Damage = new Texture2D();
	T_PlayerDead = new Texture2D();
	Platform = new Texture2D();
	Flower = new Texture2D();
	Tile = new Texture2D();
	VegetationA = new Texture2D();
	WindPlant = new Texture2D();
	BGTexture = new Texture2D();
	GreenSlime = new Texture2D();
	Orb = new Texture2D();
	BluePotion = new Texture2D();
	T_Monster_Idle = new Texture2D();
	T_Monster_Walk = new Texture2D();
	T_Monster_Attack = new Texture2D();

	T_PlayButton = new Texture2D();
	T_ExitButton = new Texture2D();
	T_MenuBG = new Texture2D();

	WizardTexture_Idle->Load("Textures/Witch/Witch_Idle.png", false);
	Platform->Load("Textures/Platform.png", false);
	Flower->Load("Textures/Flower.png", false);
	WindPlant->Load("Textures/Plants/WindPlant.png", false);
	WizardTexture_Walk->Load("Textures/Witch/Witch_Walk.png", false);
	WizardTexture_Run->Load("Textures/Witch/Witch_Run.png", false);
	WizardTexture_Jump->Load("Textures/Witch/Witch_Jump.png", false);
	_AttackTexture->Load("Textures/Witch/Witch_Swinging.png", false);
	WizardTexture_Damage->Load("Textures/Witch/Witch_Damage.png", false);
	T_PlayerDead->Load("Textures/Witch/Witch_Dead.png", false);
	Tile->Load("Textures/TileSet.png", false);
	VegetationA->Load("Textures/VegetationA.png", false);
	BGTexture->Load("Textures/BG.png", false);
	GreenSlime->Load("Textures/GreenSlime.png", false);
	Orb->Load("Textures/Orb.png", false);
	BluePotion->Load("Textures/BluePotion.png", false);
	T_Monster_Idle->Load("Textures/Enemy/Enemy_Idle.png", false);
	T_Monster_Walk->Load("Textures/Enemy/Enemy_Walk.png", false);
	T_Monster_Attack->Load("Textures/Enemy/Enemy_Swinging.png", false);

	T_PlayButton->Load("Textures/UI/PlayButton.png", false);
	T_ExitButton->Load("Textures/UI/ExitButton.png", false);
	T_MenuBG->Load("Textures/UI/MenuBG.png", false);

	WizardPosition = new Vector2(-128.0f, Graphics::GetViewportHeight()/3);
	WizardPrevPosition = new Vector2(-128.0f, Graphics::GetViewportHeight()/3);
	Velocity = new Vector2(0.0f, 0.0f);
	int t = 0;

	InitializeSequences();

	WizardRect = new Rect(0.0f, 0.0f, 847, 864);
	Tile_Rect = new Rect(512.0f, 0.0f, 512, 512);
	Tile_Rect2 = new Rect(512.0f, 1024.0f, 512, 512);
	R_HealthBar = new Rect(25, 30, 250, 20);

	// Set string position
	_stringPosition = new Vector2(10.0f, 25.0f);

	// Set Menu Paramters
	_menuBackground = new Texture2D();

	_menuBackground->Load("Textures/Transparency.png", false);

	_menuRectangle = new Rect(0.0f, 0.0f, Graphics::GetViewportWidth(), Graphics::GetViewportHeight());

	_menuStringPosition = new Vector2(Graphics::GetViewportWidth() / 2.0f, Graphics::GetViewportHeight() / 2.0f);
}

void GameManager::InitializeSequences()
{
	Sequence sequence;
	sequence.FramesCount = 14;
	sequence.grid = IdleFramesVector;
	sequence.Source = WizardTexture_Idle;
	sequence.width = 480;
	sequence.height = 480;
	IdleAnimator->Initialize(sequence);

	WalkAnimator->Initialize(WizardTexture_Walk, 28, Vector2(5, 6), 480, 480);
	RunAnimator->Initialize(WizardTexture_Run, 16, Vector2(5, 4), 480, 480);
	AttackAnimator->Initialize(_AttackTexture, 16, AttackFramesVector, 480, 480);
	JumpAnimator->Initialize(WizardTexture_Jump, 25, Vector2(5, 5), 480, 480);
	DamageAnimator->Initialize(WizardTexture_Damage, 12, Vector2(5, 3), 480, 480);

	FlowerAnimator->Initialize(Flower, 60, FlowerFramesVector, 768, 768);
	WindPlantAnimator->Initialize(WindPlant, 30, Vector2(6, 5), 512, 512);
	GreenSlimeAnimator->Initialize(GreenSlime, 30, Vector2(5, 6), 376, 256);
	OrbAnimator->Initialize(Orb, 30, Vector2(5, 6), 480, 480);

	MonsterIdleAnimator->Initialize(T_Monster_Idle, 14, Vector2(5, 3), 480, 480);
	MonsterRunAnimator->Initialize(T_Monster_Walk, 28, Vector2(5, 6), 480, 480);
	MonsterAttackAnimator->Initialize(T_Monster_Attack, 16, Vector2(5, 4), 480, 480);
}

void GameManager::Update(int elapsedTime)
{
	InputHandler(elapsedTime);
	switch (CurrentGameState)
	{
		case GameManager::GameState::Menu:
			MenuUpdate(elapsedTime);
			break;
		case GameManager::GameState::Game:
			GameUpdate(elapsedTime);
			break;
		default:
			break;
	}
}

#pragma region Menu Update
void GameManager::MenuUpdate(int elapsedTime)
{

}
#pragma endregion

#pragma region Game Update
void GameManager::GameUpdate(int elapsedTime)
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
#pragma endregion

void GameManager::Draw(int elapsedTime)
{
	SpriteBatch::BeginDraw(); // Starts Drawing

	switch (CurrentGameState)
	{
		case GameManager::GameState::Menu:
			MenuDraw(elapsedTime);
			break;
		case GameManager::GameState::Game:
			GameDraw(elapsedTime);
			break;
		default:
			break;
	}

	SpriteBatch::EndDraw(); // Ends Drawing
}

#pragma region Draw Menu Content
void GameManager::MenuDraw(int elapsedTime)
{
	std::stringstream op1;
	std::stringstream op2;
	op1 << "Play";
	op2 << "Exit";

	SpriteBatch::Draw(T_MenuBG, _menuRectangle, nullptr);
	if (MenuSelection == 0)
	{
		SpriteBatch::Draw(T_PlayButton, new Vector2(Graphics::GetViewportWidth() / 2 - 64 - 32, Graphics::GetViewportHeight() / 2 - 20), new Rect(0, 0, 128, 40), Vector2::Zero, 1.5f, 0.0f, Color::Green, SpriteEffect::NONE);
		SpriteBatch::Draw(T_ExitButton, new Vector2(Graphics::GetViewportWidth() / 2 - 64, Graphics::GetViewportHeight() / 2 + 20 + 64), new Rect(0, 0, 128, 40), Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
	}
	else
	{
		SpriteBatch::Draw(T_PlayButton, new Vector2(Graphics::GetViewportWidth() / 2 - 64, Graphics::GetViewportHeight() / 2 - 20 - 64), new Rect(0, 0, 128, 40), Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);
		SpriteBatch::Draw(T_ExitButton, new Vector2(Graphics::GetViewportWidth() / 2 - 64 - 32, Graphics::GetViewportHeight() / 2 + 20), new Rect(0, 0, 128, 40), Vector2::Zero, 1.5f, 0.0f, Color::Green, SpriteEffect::NONE);
	}
}
#pragma endregion

#pragma region Draw Game Content
void GameManager::GameDraw(int elapsedTime)
{
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

	Enemy->DrawAI(MonsterIdleAnimator, MonsterRunAnimator, MonsterAttackAnimator);
	DrawEnvironmentFront(elapsedTime);
	DrawDebugs(_drawDebug);

	SpriteBatch::DrawRectangle(new Rect(20, 25, 260, 30), Color::White); //0.443, 0.761, 0.788
	SpriteBatch::DrawRectangle(new Rect(25, 30, 250, 20), new Color(0.0f, 0.0f, 0.0f, 0.5f)); //0.443, 0.761, 0.788
	SpriteBatch::DrawRectangle(R_HealthBar, Color::Green); //0.443, 0.761, 0.788
}
#pragma endregion

void GameManager::InputHandler(int elapsedTime)
{

	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();
	Input::MouseState* mouseState = Input::Mouse::GetState();
	switch (CurrentGameState)
	{
		case GameManager::GameState::Menu:
			MenuInput(keyboardState, mouseState);
			break;
		case GameManager::GameState::Game:
			if (PState != PlayerState::Dead)
				GameInput(keyboardState, mouseState);
			break;
		default:
			break;
	}
}

void GameManager::MenuInput(Input::KeyboardState* keyboardState, Input::MouseState* mouseState)
{
	if (keyboardState->IsKeyDown(Input::Keys::W) || keyboardState->IsKeyDown(Input::Keys::UP))
	{
		MenuSelection = 0;
	}
	else if (keyboardState->IsKeyDown(Input::Keys::S) || keyboardState->IsKeyDown(Input::Keys::DOWN))
	{
		MenuSelection = 1;
	}

	if (keyboardState->IsKeyDown(Input::Keys::RETURN))
	{
		if (MenuSelection)
			exit(0);
		else
			CurrentGameState = GameState::Game;
	}
}

void GameManager::GameInput(Input::KeyboardState* keyboardState, Input::MouseState* mouseState)
{
	if (PState == PlayerState::Attacking)
		return;


	if (!_paused)
	{
		if (keyboardState->IsKeyDown(Input::Keys::W))
		{
			PState = PlayerState::Idle;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::A))
		{
			Velocity->X = -WizardSpeed;
			PState = PlayerState::Walk;
			isFlipped = false;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::S))
		{
			PState = PlayerState::Idle;
		}
		else if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			Velocity->X = WizardSpeed;
			PState = PlayerState::Walk;
			isFlipped = true;
		}
		else if (mouseState->LeftButton == Input::ButtonState::PRESSED)
			PState = PlayerState::Attacking;
		else
		{
			if (PState != PlayerState::Damage && PState != PlayerState::Dead)
				PState = PlayerState::Idle;
			Velocity->X = 0.0f;
		}


		if (keyboardState->IsKeyDown(Input::Keys::SPACE))
		{
			Jump();
		}

		if (keyboardState->IsKeyDown(Input::Keys::LEFTSHIFT))
		{
			PState = PlayerState::Running;
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
	{
		_drawDebug = true;
	}
}

void GameManager::DrawPlayerAnimation(int elapsedTime)
{
	Sleep(40);

	switch (PState)
	{
		case PlayerState::Idle:
			IdleAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Walk:
			WalkAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Running:
			RunAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Jumping:
			JumpAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Attacking:
			if (AttackAnimator->PlaySequenceOnce(WizardPosition, isFlipped, 0.8f))
			{
				Enemy->Damage(0.0f);
				PState = PlayerState::Idle;
			}
			break;
		case PlayerState::Damage:
			DamageAnimator->PlaySequence(WizardPosition, isFlipped, 0.8f);
			break;
		case PlayerState::Dead:
			SpriteBatch::Draw(T_PlayerDead, WizardPosition , new Rect(0, 0, 480, 480), Vector2::Zero, 0.8f, 0.0f, Color::White, isFlipped ? SpriteEffect::FLIPHORIZONTAL : SpriteEffect::NONE);
			break;
	}
}

void GameManager::DrawEnvironmentFront(int elapsedTime)
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
	
	//SpriteBatch::Draw(BluePotion, new Vector2((Graphics::GetViewportWidth() / 2), (Graphics::GetViewportHeight() - 150)), new Rect(128.0f, 128.0f, 128.0f, 128.0f), Vector2::Zero, 0.5f, 0.0f, Color::White, SpriteEffect::NONE);
	
	//OrbAnimator->PlaySequence(new Vector2(Graphics::GetViewportWidth() / 2.0f - 200, (Graphics::GetViewportHeight() - 280)), false, 0.3f);
}

void GameManager::DrawEnvironmentBack(int elapsedTime)
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

void GameManager::Jump()
{
	if (collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height + 1.0f)) ||
		collisionManager->IsCollider(Vector2(PlayerCollider->Rect->X + PlayerCollider->Rect->Width, PlayerCollider->Rect->Y + PlayerCollider->Rect->Height + 1.0f)))
		Velocity->Y = -JumpForce;
}

void GameManager::Dash()
{
	if (isFlipped)
		Velocity->X = RunSpeed;
	else
		Velocity->X = -RunSpeed;
}

void GameManager::Damage(float damage)
{
	if (*Health > 0)
	{
		PState = PlayerState::Damage;

		float N_HealthBar = R_HealthBar->Width / 250.0f;
		*Health -= 0.08f;
		R_HealthBar->Width = *Health * 250.0f;
	}
	else
	{
		R_HealthBar->Width = 0.0f;
		PState = PlayerState::Dead;
	}
}

void GameManager::SetupCollisions()
{
	PlayerCollider = new Collision(Collision::CollisionType::Dynamic);
	PlayerCollider->Rect->Width = 100.0f;
	PlayerCollider->Rect->Height = 150.0f;
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

void GameManager::DrawDebugs(bool draw)
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

int GameManager::random(int min, int max)
{
	static bool first = true;
	if (first)
	{
		srand(time(NULL));
		first = false;
	}
	return min + rand() % ((max + 1) - min);
}

void GameManager::AudioHanlder()
{
	BG_Music = new SoundEffect(true, 1.0f, 0.2f);
	BG_Music->Load("Audio/BG_Music.wav");
	Audio::Play(BG_Music);
}