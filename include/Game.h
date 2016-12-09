/// <summary>
/// @author RP / Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "LevelLoader.h"
#include "Tank.h"
#include "KeyHandler.h"
#include "CollisionDetector.h"
#include "MenuLoader.h"
#include "Target.h"
#include <iostream>
#include <fstream>

enum GameState {Play, Menu, Pause, GameOver, PlayLevel};

/// <summary>
/// @brief Main class for the SFML Playground project.
/// This will be a single class framework for learning about SFML.
/// </summary>
class Game
{
public:
	Game();	// Game constructor
	void run(); // Game run method
protected:	
	//method declarations
	void update(double dt);
	void render();
	void processEvents();	
	void processGameEvents(sf::Event&);
	void generateWalls(int);
	bool CheckTankWallCollision();
	void CheckBulletWallCollision();
	void updateGame(double dt);
	void updateMenu();
	void UpdateGameOver();
	void updateLevelSelect();
	void renderGame();
	void renderMenu();
	void renderGameOver();
	void renderLevelSelect();
	bool CheckTankTargetCollision();
	void CheckBulletTargetCollision();
	bool checkTargetsDead();
	void initTank(int);
	void initText();
	void initMenu();
	void generateTargets(int);
	void resetGame(int level);
	void initIndividualText(sf::Text &, int, int, int);
	void fileReader();

	//Objects and variables
	//Render Window
	sf::RenderWindow m_window;

	//Statistics
	int score;
	int currentTarget;
	int currentLevel;
	int m_shotsFired;
	int m_hits;
	float m_accuracy = 0;

	//Textures
	sf::Texture m_Texture;
	sf::Texture m_BackgroundTexture;
	sf::Texture m_menuBackTexture;
	sf::Texture m_targetTexture;

	//Sprites
	sf::Sprite m_BackgroundSprite;
	sf::Sprite m_menuBackSprite;

	//Last and Current Key states
	KeyHandler m_KeyHandler = KeyHandler();
	KeyHandler m_lastKeyStates = m_KeyHandler;	

	//Font and Texts
	sf::Font m_font;
	sf::Text m_timeText;
	sf::Text m_heading;
	sf::Text m_accuracyText;
	sf::Text m_scoreText;
	sf::Text m_gameOverText;
	sf::Text m_resetText;

	//Sounds and Sound Buffers
	sf::SoundBuffer m_gunShotBuffer;
	sf::SoundBuffer m_ricochetBuffer;
	sf::SoundBuffer m_backingTrackBuffer;
	sf::Sound m_gunShotSound;
	sf::Sound m_backingTrack;
	sf::Sound m_ricochet;

	//Vectors
	std::vector<Target> m_targets; // Target vector
	std::vector<sf::Text> m_menuTexts; // Menu text vector
	std::vector<bool> m_menuTextStates; // Menu text states vector (for deciding which is red and where the player is)
	std::vector<sf::Text> m_levelTexts; // Level texts vector
	std::vector<bool> m_levelTextStates; // Level texts states vector
	std::vector<std::unique_ptr<sf::Sprite>> m_wallSprites; // wall sprites vector
	LevelData m_Level[3]; // Levels
	MenuLoader m_menuLoader; // Menu
	sf::Clock m_countDownClock; // Game Clock
	GameState gameState = Menu; // Gamestate
	std::unique_ptr<Tank> m_Tank; // Tank
	//HighScores
	int m_highScore;
	int m_highAccuracy;
};