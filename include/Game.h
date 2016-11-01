#pragma once

#include <SFML/Graphics.hpp>
#include "LevelLoader.h"
#include "Tank.h"
#include "KeyHandler.h"

/// <summary>
/// @brief Main class for the SFML Playground project.
/// 
/// This will be a single class framework for learning about SFML.
/// </summary>
class Game
{
public:
	Game();	
	void run();

protected:	
	void update(double dt);
	void render();
	void processEvents();	
	void processGameEvents(sf::Event&);
	
	// main window
	sf::RenderWindow m_window;

	sf::Sprite m_Sprite;
	sf::Texture m_Texture;

	sf::Sprite m_BackgroundSprite;
	sf::Texture m_BackgroundTexture;

	LevelData m_Level;

	//Vector is like a list (c#)
	std::vector<sf::Sprite> m_sprites;
	
	//KeyHandler Object
	KeyHandler m_KeyHandler = KeyHandler();

	//Unique Pointer to Tank
	std::unique_ptr<Tank> m_Tank;


};
