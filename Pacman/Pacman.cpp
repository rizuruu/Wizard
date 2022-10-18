#include "Pacman.h"

#include <sstream>

Pacman::Pacman(int argc, char* argv[]) : Game(argc, argv), _cPacmanSpeed(0.1f) 
{
	_frameCount = 0;
	_paused = false;
	_pKeyDown = false;

	//Initialise important Game aspects
	Graphics::Initialise(argc, argv, this, Graphics::GetViewportWidth(), Graphics::GetViewportHeight(), false, 25, 25, "Pacman", 60);
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
	// Load Pacman
	_pacmanTexture = new Texture2D();
	_pacmanTexture->Load("Textures/PacmanNew.png", false);
	_pacmanPosition = new Vector2(350.0f, 350.0f);
	_pacmanSourceRect = new Rect(0.0f, 0.0f, 28, 39);
	_pacmanIdle2Rect = new Rect(28.0f, 0.0f, 28, 39);
	_pacmanRun1Rect = new Rect(84.0f, 0.0f, 28, 39);
	_pacmanRun2Rect = new Rect(112.0f, 0.0f, 28, 39);

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
	// Gets the current state of the keyboard
	Input::KeyboardState* keyboardState = Input::Keyboard::GetState();

	if (!_paused)

	{

		// Checks if D key is pressed

		if (keyboardState->IsKeyDown(Input::Keys::D))
		{
			_pacmanPosition->X += _cPacmanSpeed * elapsedTime;
			PState = PlayerState::Walking;
		}
		else
		{
			PState = PlayerState::Idle;
		}

		// Checks if Pacman is trying to disappear

		if (_pacmanPosition->X > Graphics::GetViewportWidth())

		{

			// Pacman hit right wall - reset his position

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

void Pacman::Draw(int elapsedTime)
{
	// Allows us to easily create a string
	std::stringstream stream;
	stream << "Pacman X: " << _pacmanPosition->X << " Y: " << _pacmanPosition->Y;

	SpriteBatch::BeginDraw(); // Starts Drawing


	if (_paused)

	{
		std::stringstream menuStream; menuStream << "PAUSED!";
		SpriteBatch::Draw(_menuBackground, _menuRectangle, nullptr); SpriteBatch::DrawString(menuStream.str().c_str(), _menuStringPosition, Color::Red);
	}

	if (_frameCount < 30)
	{
		// Draws Red Munchie
		SpriteBatch::Draw(_munchieInvertedTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		switch (PState)
		{
			case Pacman::PlayerState::Idle:
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanSourceRect); // Draws Pacman
				break;
			case Pacman::PlayerState::Walking:		
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanRun1Rect); // Draws Pacman
				break;
			default:
				break;
		}

		_frameCount++;
	}
	else
	{
		// Draw Blue Munchie
		SpriteBatch::Draw(_munchieBlueTexture, _munchieRect, nullptr, Vector2::Zero, 1.0f, 0.0f, Color::White, SpriteEffect::NONE);

		switch (PState)
		{
			case Pacman::PlayerState::Idle:
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanIdle2Rect); // Draws Pacman
				break;
			case Pacman::PlayerState::Walking:
				SpriteBatch::Draw(_pacmanTexture, _pacmanPosition, _pacmanRun2Rect); // Draws Pacman
				break;
			default:
				break;
		}

		_frameCount++;

		if (_frameCount >= 60)
			_frameCount = 0;
	}	
	// Draws String
	SpriteBatch::DrawString(stream.str().c_str(), _stringPosition, Color::Green);
	SpriteBatch::EndDraw(); // Ends Drawing
}