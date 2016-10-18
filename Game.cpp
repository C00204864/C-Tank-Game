#include "Game.h"

/// <summary>
/// @author RP
/// @date June 2016
/// @version 1.0
/// 
/// </summary>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

/// <summary>
/// @brief Default constructor.
/// 
/// Create a new window and initialise member objects.
/// </summary>
/// 
Game::Game()
: m_window(sf::VideoMode(1440, 900, 32), "SFML Playground", sf::Style::Default)
{	
	//Load Level
	int currentLevel = 1;
	if (!LevelLoader::load(currentLevel, m_Level))
	{
		return;
	}

	//Tank Sprite
	if (!m_Texture.loadFromFile("E-100.png"))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	m_Sprite.setTexture(m_Texture);
	sf::Vector2u m_Vector;
	m_Vector = m_window.getSize();
	m_Sprite.setPosition(m_Level.m_tank.m_position);
	m_Vector = m_Texture.getSize();
	m_Sprite.setOrigin(m_Vector.x / 2, m_Vector.y / 2);
	
	//Background Sprite
	if (!m_BackgroundTexture.loadFromFile(m_Level.m_background.m_fileName))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	m_BackgroundSprite.setTexture(m_BackgroundTexture);

	sf::Sprite tempSprite;
	tempSprite.setTexture(m_Texture);
	for (auto &obstacle : m_Level.m_obstacles)
	{
		tempSprite.setPosition(obstacle.m_position);
		tempSprite.rotate(obstacle.m_rotation);
		m_sprites.push_back(tempSprite);
	}
}

/// <summary>
/// Main game entry point - runs until user quits.
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;

	while (m_window.isOpen())
	{
		sf::Time dt = clock.restart();

		lag += dt.asMilliseconds();

		processEvents();

		while (lag > MS_PER_UPDATE)
		{
			update(MS_PER_UPDATE);
			lag -= MS_PER_UPDATE;
		}
		update(MS_PER_UPDATE);

		render();
	}
}



/// <summary>
/// @brief Check for events
/// 
/// Allows window to function and exit. 
/// Events are passed on to the Game::processGameEvents() method
/// </summary>
void Game::processEvents()
{
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed)
		{
			m_window.close();
		}
		processGameEvents(event);
	}
}

/// <summary>
/// @brief Handle all user input.
/// 
/// Detect and handle keyboard input.
/// </summary>
/// <param name="event">system event</param>
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type)
	{
		switch (event.key.code)
		{
		case sf::Keyboard::Escape:
			m_window.close();
			break;
		case sf::Keyboard::Up:
			m_Sprite.rotate(90);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// Method to handle all game updates
/// </summary>
/// <param name="time">update delta time</param>
void Game::update(double dt)
{
	
}


/// <summary>
/// @brief draw the window for the game.
/// 
/// </summary>
void Game::render()
{
	m_window.clear(sf::Color(0, 0, 0, 0));
	m_window.draw(m_BackgroundSprite);
	m_window.draw(m_Sprite);
	for (auto &sprite : m_sprites)
	{
		m_window.draw(sprite);
	}
	m_window.display();
}


