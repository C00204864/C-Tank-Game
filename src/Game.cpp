#include "Game.h"

/// <summary>
/// @author RP / Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

// Updates per milliseconds
static double const MS_PER_UPDATE = 10.0;

/// <summary>
/// @brief Default constructor.
/// Create a new window and initialise member objects.
/// </summary>
Game::Game()
: m_window(sf::VideoMode(1440, 900, 32), "SFML Playground", sf::Style::Default),
score(0), 
currentTarget(0),
currentLevel(1),
m_shotsFired(0),
m_hits(0)
{	
	m_window.setVerticalSyncEnabled(true);
	
	//Load Level
	if (!LevelLoader::load(currentLevel, m_Level[0]) || !LevelLoader::load(currentLevel + 1, m_Level[1]) || !LevelLoader::load(currentLevel + 2, m_Level[2]) || !m_menuLoader.load())
	{
		return; // We return if the level cannot be loaded (program crash)
	}
	//Background Sprite
	if (!m_BackgroundTexture.loadFromFile(m_Level[0].m_background.m_fileName))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	initText(); // Initialise the game text
	initMenu(); // Initialise the main menu
	//Load and initialise sound
	if (!m_ricochetBuffer.loadFromFile("resources/sounds/ricochet.wav"))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	m_ricochet.setBuffer(m_ricochetBuffer);
	if (!m_gunShotBuffer.loadFromFile("resources/sounds/gunshot.wav"))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	m_gunShotSound.setBuffer(m_gunShotBuffer);
	if (!m_backingTrackBuffer.loadFromFile("resources/sounds/backingtrack.wav"))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	m_backingTrack.setBuffer(m_backingTrackBuffer);
	m_backingTrack.setLoop(true); //We set the backing track to loop repeatedly forever
	m_backingTrack.setVolume(30); //Lower the volume
	initTank(currentLevel);
}

/// <summary>
/// @brief Function used to update and draw the game
/// </summary>
void Game::run()
{
	sf::Clock clock;
	sf::Int32 lag = 0;
	m_backingTrack.play(); //Start the backing track outside of the game loop
	m_countDownClock.restart();
	clock.restart();
	while (m_window.isOpen())
	{
		while (lag > MS_PER_UPDATE)
		{
			processEvents();
			update(clock.restart().asMilliseconds());
			lag -= MS_PER_UPDATE;
		}
		processEvents();
		sf::Time dt = clock.restart();
		lag += dt.asMilliseconds();
		update(dt.asMilliseconds());
		render();
	}
}

/// <summary>
/// @brief Check for events
/// Allows window to function and exit. 
/// Events are passed on to the Game::processGameEvents() method
/// </summary>
void Game::processEvents()
{
	m_lastKeyStates = m_KeyHandler;
	sf::Event event;
	while (m_window.pollEvent(event))
	{
		if (event.type == sf::Event::Closed) // Check for a window close event
		{
			m_window.close(); // Close the window ad thus the game
		}
		processGameEvents(event);
	}	
}

/// <summary>
/// @brief Handle all user input.
/// Detect and handle keyboard input.
/// </summary>
/// <param name="event">system event</param>
void Game::processGameEvents(sf::Event& event)
{
	// check if the event is a a mouse button release
	if (sf::Event::KeyPressed == event.type || sf::Event::KeyReleased == event.type)
	{
		//Switch updates the keyhandler
		switch (event.type)
		{
		case sf::Event::KeyPressed:
			m_KeyHandler.updateKey(event.key.code, true);
			break;
		case sf::Event::KeyReleased:
			m_KeyHandler.updateKey(event.key.code, false);
			break;
		default:
			break;
		}
	}
}

/// <summary>
/// @brief Function updates all of the in game objects based
/// on the time that is passed to it in the run function
/// </summary>
/// <param name="time">update delta time</param>
void Game::update(double dt)
{
	//Switch stement updates the game based on the current game state
	switch (gameState)
	{
	case Play:
		updateGame(dt);
		break;
	case Menu:
		updateMenu();
		break;
	case GameOver:
		UpdateGameOver();
		break;
	case PlayLevel:
		updateLevelSelect();
		break;
	}
}

/// <summary>
/// @brief Function updates all of the game objects and variables
/// during normal play
/// </summary>
/// <param name="dt">delta time is the time passed since the last update</param>
void Game::updateGame(double dt)
{
	m_Tank->update(dt, (CheckTankWallCollision() || CheckTankTargetCollision())); // Update the tank with its collision status as well as delta time
	for (int i = 0; i < m_targets.size(); i++)
	{
		m_targets.at(i).update(dt);
	}
	//Check for collisions
	CheckBulletWallCollision();
	CheckBulletTargetCollision();
	//Handle Strings
	std::ostringstream ss;
	//Time
	ss << "Time: " << static_cast<int>(60 - m_countDownClock.getElapsedTime().asSeconds());
	m_timeText.setString(ss.str());
	ss.str(std::string()); // clear the string stream
	//Score
	ss << "Score: " << score;
	m_scoreText.setString(ss.str());
	//Accuracy
	if (m_shotsFired != 0)
	{
		m_accuracy = (100 * static_cast<float>(m_hits) / m_shotsFired);
	}
	ss.str(std::string());
	ss << "Accuracy: " << static_cast<int>(m_accuracy) << "%";
	m_accuracyText.setString(ss.str());
	ss.str(std::string());

	//Endgame
	if (m_countDownClock.getElapsedTime().asSeconds() > 60 || checkTargetsDead())
	{
		gameState = GameOver; // Change the game state
		score += (60 - m_countDownClock.getElapsedTime().asSeconds()); // Final score
		fileReader();
		ss << "     Accuracy: " << static_cast<int>(m_accuracy) << "%       " <<  score <<  m_Level[currentLevel - 1].m_resetString.m_string << "\n		                      High Score: " << m_highScore << "\n                          High Accuracy: " << m_highAccuracy; // Endgame string
		m_resetText.setString(ss.str()); // Set the string to the resetText object
		
	}
}

/// <summary>
/// @brief Function updates all variables objects associated with the menu
/// </summary>
void Game::updateMenu()
{
	//Check key presses
	if (m_KeyHandler.isPressed(sf::Keyboard::Return) && !m_lastKeyStates.isPressed(sf::Keyboard::Return)) // Deal with a return key press
	{
		if (m_menuTextStates.at(0)) // Enter a new game
		{
			currentLevel = 1;
			resetGame(currentLevel);
			m_countDownClock.restart();
			gameState = Play;
		}
		else if (m_menuTextStates.at(1)) // Enter level selection
		{
			gameState = PlayLevel;
		}
		else // Exit the game
		{
			m_window.close();
		}
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Down) && !m_lastKeyStates.isPressed(sf::Keyboard::Down)) // Deal with a down key press
	{
		for (int i = 0; i < m_menuTexts.size(); i++) // Check where the player has currently selected
		{
			if (m_menuTextStates.at(i))
			{
				m_menuTextStates.at(i) = false;
				if (m_menuTexts.size() == i + 1)
				{
					m_menuTextStates.at(0) = true; // If the player is at the end of the menu move them back to the start
				}
				else
				{
					m_menuTextStates.at(i + 1) = true; // Otherwise move the player down one step
				}
				break; // Break out of the for loop once the action has been carried out
			}
		}
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Up) && !m_lastKeyStates.isPressed(sf::Keyboard::Up)) // Deal with an up key press(very similar to the down key press section)
	{
		for (int i = 0; i < m_menuTexts.size(); i++)
		{
			if (m_menuTextStates.at(i))
			{
				m_menuTextStates.at(i) = false;
				if (i == 0)
				{
					m_menuTextStates.at(m_menuTexts.size() - 1) = true; // If the player is at the start of the menu move them to the end
				}
				else
				{
					m_menuTextStates.at(i - 1) = true; // Otherwise move them up one
				}
				break; // Break out of the for loop once the action has been carried out
			}
		}
	}
}

/// <summary>
/// @brief Function updates key presses nfor the game over screen
/// and determines what the next game state is
/// </summary>
void Game::UpdateGameOver()
{
	if (m_KeyHandler.isPressed(sf::Keyboard::Return) && currentLevel < 3) //If the return key is pressed and the level is not 3 then move to the next level
	{
		currentLevel++; // Increment the level count
		resetGame(currentLevel); // Reset the game
		gameState = Play; // Change the game state
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Escape)) // If the escape key is pressed return the player to the main menu
	{
		gameState = Menu; // Change the game state
	}
}

/// <summary>
/// @brief Function works in a very similar way to the menu update
/// function in that it allows the player to scroll through a menu
/// and select an option
/// </summary>
void Game::updateLevelSelect()
{
	// Function allows the user to use numbers to select their level as well as the normal menu method
	if (m_KeyHandler.isPressed(sf::Keyboard::Num1) || m_KeyHandler.isPressed(sf::Keyboard::Numpad1)) // If the number one is pressed
	{
		currentLevel = 1; //Set the level
		resetGame(currentLevel);  //Set up a game
		gameState = Play; // Change the game
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Num2) || m_KeyHandler.isPressed(sf::Keyboard::Numpad2)) // If the number two is pressed
	{
		currentLevel = 2;
		resetGame(currentLevel);
		gameState = Play;
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Num3) || m_KeyHandler.isPressed(sf::Keyboard::Numpad3)) // If the number three is pressed
	{
		currentLevel = 3;
		resetGame(currentLevel);
		gameState = Play;
	}
	if (m_KeyHandler.isPressed(sf::Keyboard::Return) && !m_lastKeyStates.isPressed(sf::Keyboard::Return)) //Deal with a return key press
	{
		if (m_levelTextStates.at(0)) // Level One
		{
			currentLevel = 1;
			resetGame(currentLevel);
			gameState = Play;
		}
		else if (m_levelTextStates.at(1)) // Level Two
		{
			currentLevel = 2;
			resetGame(currentLevel);
			gameState = Play;
		}
		else // Level Three
		{
			currentLevel = 3;
			resetGame(currentLevel);
			gameState = Play;
		}
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Down) && !m_lastKeyStates.isPressed(sf::Keyboard::Down)) //Deal with a down key press
	{
		//Functions in the exact same way as the menu
		for (int i = 0; i < m_menuTexts.size(); i++)
		{
			if (m_levelTextStates.at(i))
			{
				m_levelTextStates.at(i) = false;
				if (m_levelTexts.size() == i + 1)
				{
					m_levelTextStates.at(0) = true;
				}
				else
				{
					m_levelTextStates.at(i + 1) = true;
				}
				break;
			}
		}
	}
	else if (m_KeyHandler.isPressed(sf::Keyboard::Up) && !m_lastKeyStates.isPressed(sf::Keyboard::Up)) //Deal with an up key press
	{
		//Functions in the exact same way as the menu
		for (int i = 0; i < m_levelTexts.size(); i++)
		{
			if (m_levelTextStates.at(i))
			{
				m_levelTextStates.at(i) = false;
				if (i == 0)
				{
					m_levelTextStates.at(m_levelTexts.size() - 1) = true;
				}
				else
				{
					m_levelTextStates.at(i - 1) = true;
				}
				break;
			}
		}
	}
}

/// <summary>
/// @brief Function usewd to draw all objects in the game as a whole
/// </summary>
void Game::render()
{
	//Clear the screen
	m_window.clear(sf::Color(0, 0, 0, 0));
	//Switch statement draws the game based on the current game state
	switch (gameState)
	{
	case Play:
		renderGame();
		break;
	case Menu:
		renderMenu();
		break;
	case GameOver:
		renderGameOver();
		break;
	case PlayLevel:
		renderLevelSelect();
		break;
	}
	//Display all draw calls
	m_window.display();
}

/// <summary>
/// @brief Function draw all objects in the game during normal game time
/// </summary>
void Game::renderGame()
{
	m_window.draw(m_BackgroundSprite); // Draw the background
	for (auto &sprite : m_wallSprites) // Draw the obstacles with a for loop
	{
		m_window.draw(*sprite);
	}
	if (currentTarget < m_targets.size()) // Draw the targets with a for loop
	{
		m_targets.at(currentTarget).render(m_window);
	}
	m_Tank->render(m_window); // Call the tank render function
	m_window.draw(m_timeText); //Draw HUD elements
	m_window.draw(m_scoreText);
	m_window.draw(m_accuracyText);
}

/// <summary>
/// @brief function renders the main menu if the game
/// state is in the main menu state (function will
/// determine which piece of text is selected and paint 
/// it red)
/// </summary>
void Game::renderMenu()
{
	m_window.draw(m_menuBackSprite); // Draw the menu background sprite
	m_window.draw(m_heading); // Draw text(heading)
	for (int i = 0; i < m_menuTexts.size(); i++) // Draw the elements of the menu using a for loop
	{
		if (m_menuTextStates.at(i) && m_menuTexts.at(i).getColor() == sf::Color::White)// Decide the color based on where the player is
		{
			m_menuTexts.at(i).setColor(sf::Color::Red); // Set to red
		}
		else if (!m_menuTextStates.at(i) && m_menuTexts.at(i).getColor() == sf::Color::Red)
		{
			m_menuTexts.at(i).setColor(sf::Color::White); // Otherwise it's white
		}
		m_window.draw(m_menuTexts.at(i)); // The draw call
	}
}

/// <summary>
///  @brief Function draws all elemst of the game over screen
/// </summary>
void Game::renderGameOver()
{
	m_window.draw(m_menuBackSprite); //Draw the background
	m_window.draw(m_gameOverText); //Draw the BIG Game Over text
	m_window.draw(m_resetText); //Draw the rest of the text
}

/// <summary>
/// @brief Function draws all of the text for the level
/// selector, functions just like the renderMenu function
/// </summary>
void Game::renderLevelSelect()
{
	m_window.draw(m_menuBackSprite);
	for (int i = 0; i < m_levelTexts.size(); i++)
	{
		if (m_levelTextStates.at(i) && m_levelTexts.at(i).getColor() == sf::Color::White) //Set Color if the player's position is on one of the elements of the menu
		{
			m_levelTexts.at(i).setColor(sf::Color::Red);
		}
		else if (!m_levelTextStates.at(i) && m_levelTexts.at(i).getColor() == sf::Color::Red) //Otherwise it must be white
		{
			m_levelTexts.at(i).setColor(sf::Color::White);
		}
		m_window.draw(m_levelTexts.at(i)); //Draw the text
	}
}

/// <summary>
/// @brief function generates the in game obstacles by reading
///  the level loader and creating the relevant amount of
/// sprites in the game
/// </summary>
void Game::generateWalls(int level)
{
	sf::Sprite tempSprite;
	tempSprite.setTexture(m_Texture); // Set the texture for the walls
	sf::IntRect wallRect(2, 129, 33, 23); //Set the rectangle of the sprite we wish to draw
	//Create the walls
	for (ObstacleData const &obstacle : m_Level[level - 1].m_obstacles)
	{
		std::unique_ptr<sf::Sprite> sprite(new sf::Sprite());
		sprite->setTexture(m_Texture);
		sprite->setTextureRect(wallRect);
		sprite->setOrigin(wallRect.width / 2.0, wallRect.height / 2.0);
		sprite->setPosition(obstacle.m_position);
		sprite->setRotation(obstacle.m_rotation);
		m_wallSprites.push_back(std::move(sprite));
	}
}

/// <summary>
/// @brief checks for a collision between the Player Tank
/// and the in-game walls
/// </summary>
/// <returns>bollean telling whether or not the tank was hit</returns>
bool Game::CheckTankWallCollision()
{
	for (std::unique_ptr<sf::Sprite> const & sprite : m_wallSprites) //For loop to check every wall
	{
		if (CollisionDetector::collision(m_Tank->GetTurretSprite(), *sprite)) //Check the collision between the two sprites
		{
			return true; // Return true if there's a collision
		}
	}
	return false; // Otherwise return false
}


/// <summary>
/// @brief function checksfor any collisions between
/// fired bulletes and the in game walls and 
/// automatically kills the relevant bullet
/// </summary>
void Game::CheckBulletWallCollision()
{
	for (int i = 0; i < Tank::NUMBER_OF_BULLETS; i++) // For loop to check every Bullet
	{
		for (std::unique_ptr<sf::Sprite> const & sprite : m_wallSprites) // For loop to check every wall
		{
			if (CollisionDetector::collision(Tank::m_bulletVector.at(i).m_sprite, *sprite)) //Check for a collision between the sprites
			{
				Tank::m_bulletVector.at(i).Collision(); // Call the collision function in the relevant bullet to disable it
			}
		}
	}
}

/// <summary>
/// @brief Function is used to determine if there was
/// a collision between the Player Tank and any of
/// the targets in the game
/// </summary>
/// <returns>boolean determining of there was a collision</returns>
bool Game::CheckTankTargetCollision()
{
	if (currentTarget < m_targets.size() && CollisionDetector::collision(m_Tank->GetTurretSprite(), m_targets.at(currentTarget).getSprite())) //Check if the tank has collidied with the active target
	{
		return true;
	}
	return false;
}

/// <summary>
/// @brief Function used to check collisions between bullets
/// and targets in the game using the CollisionDetector class,
/// the function will automatically kill the bullet and the 
/// target
/// </summary>
void Game::CheckBulletTargetCollision()
{
	for(int i = 0; i < Tank::NUMBER_OF_BULLETS; i++)
	{
		if (currentTarget < m_targets.size() && CollisionDetector::collision(m_targets.at(currentTarget).getSprite(), Tank::m_bulletVector.at(i).m_sprite) && Tank::m_bulletVector.at(i).fired) //If the bullet is active and colliding
		{
			m_hits++; // Increment hits for accuracy
			Tank::m_bulletVector.at(i).Collision(); // Call the collision function in the bullet
			m_targets.at(currentTarget).hit(); // Call the hit function in the target
			m_ricochet.play(); //Play a sound
		}
	}
}

/// <summary>
/// @brief function simply initiates the player tank whenever
/// it needs to be initialised
/// </summary>
void Game::initTank(int level)
{
	//Load Tank Sprite
	if (!m_Texture.loadFromFile("./resources/images/SpriteSheet.png"))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	m_Tank.reset(new Tank(m_Texture, m_Level[level - 1].m_tank.m_position, m_KeyHandler, m_gunShotSound, &m_shotsFired)); //Initialise the tank
}

/// <summary>
/// @brief simple mfunction used to initialise text for 
/// the in-game HUD so as to allevite the size of the Game
/// constructor function
/// </summary>
void Game::initText()
{
	if (m_font.loadFromFile(m_menuLoader.m_font.m_filename)) // Attempt to load the font
	{
		// If successful initialise the text
		m_gameOverText.setString("Game Over"); // String does not change so we set it here
		initIndividualText(m_timeText, 30, 100, 50);
		initIndividualText(m_scoreText, 30, 1200, 50);
		initIndividualText(m_gameOverText, 120, 400, 200);
		initIndividualText(m_resetText, 30, 380, 400);
		initIndividualText(m_accuracyText, 30, 1200, 100);
	}
	else // Otherwise there was an error loading the file
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
}

/// <summary>
/// Function simply initialises text passed to it by refernce
/// </summary>
/// <param name="textIn">The text to be initialised</param>
/// <param name="charSize">The desired character size</param>
/// <param name="posX">The desired X position of the text</param>
/// <param name="posY">The desired Y position of the text</param>
void Game::initIndividualText(sf::Text & textIn, int charSize, int posX, int posY)
{
	textIn.setCharacterSize(charSize); // Set the character size
	textIn.setPosition(posX, posY); // Set the position
	textIn.setColor(sf::Color::White); // Set the color
	textIn.setFont(m_font); // Set the font
}

/// <summary>
/// @brief function used to generate the targets within
///  a particular level by going through and checking the
/// targets vector of the level-loader
/// </summary>
void Game::generateTargets(int level)
{
	//Load the target texture
	if (!m_targetTexture.loadFromFile(m_Level[level - 1].m_targetFile.m_filename))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	for (int i = 0; i < m_Level[level - 1].m_targets.size(); i++) //Use a for lopp to initialise and push back all of the targets
	{
		m_targets.push_back(Target(m_targetTexture,
			m_Level[level - 1].m_targets.at(i).m_position,
			m_Level[level - 1].m_targets.at(i).m_rotation,
			m_Level[level - 1].m_targets.at(i).m_duration,
			m_Level[level - 1].m_targets.at(i).m_offset,
			score,
			currentTarget));
	}
}

/// <summary>
/// @brief simple function used to initiate text 
/// involved with the main menu and alleviate the
/// size of the Game constructor function
/// </summary>
void Game::initMenu()
{
	sf::Text tempText;
	for (int i = 0; i < m_menuLoader.m_textArray.size(); i++) // For loop to initialise all of the text in the main menu
	{
		tempText.setString(m_menuLoader.m_textArray.at(i).m_text); // Set the string
		tempText.setPosition(m_menuLoader.m_textArray.at(i).m_position); // Set the position
		tempText.setFont(m_font); // Set the font
		tempText.setCharacterSize(30); // Set the character size
		tempText.setColor(sf::Color::White); // Set the color
		m_menuTexts.push_back(tempText); // Push it to the vector
		m_menuTextStates.push_back(m_menuLoader.m_textArray.at(i).m_state);
	}
	for (int i = 0; i < m_menuLoader.m_levelsTextArray.size(); i++) // For loop to initialise all of the text in the level selector
	{
		//functions similarly to the previous for loop
		tempText.setString(m_menuLoader.m_levelsTextArray.at(i).m_text);
		tempText.setPosition(m_menuLoader.m_levelsTextArray.at(i).m_position);
		tempText.setFont(m_font);
		tempText.setCharacterSize(30);
		tempText.setColor(sf::Color::White);
		m_levelTexts.push_back(tempText);
		m_levelTextStates.push_back(m_menuLoader.m_levelsTextArray.at(i).m_state);
	}
	if (!m_menuBackTexture.loadFromFile(m_menuLoader.m_background.m_filename))
	{
		std::string s = "Error Loading From File";
		throw std::exception(s.c_str());
	}
	//Set up the background sprites
	m_menuBackSprite.setTexture(m_menuBackTexture);
	m_BackgroundSprite.setTexture(m_BackgroundTexture);
	//Initialise the title heading
	m_heading.setString(m_menuLoader.m_heading.m_text);
	m_heading.setFont(m_font);
	m_heading.setPosition(m_menuLoader.m_heading.m_position);
}

/// <summary>
/// @brief Function simply checks whether each and every
/// target has been hit(Dead)
/// </summary>
/// <returns>boolean (true if all targets have been hit in a level)</returns>
bool Game::checkTargetsDead()
{
	for (int i = 0; i < m_targets.size(); i++) // For loop checks all of the targets
	{
		if (m_targets.at(i).getState()) // Get the state
		{
			return false; // We return false if they are living
		}
	}
	return true;
}

/// <summary>
/// @brief Function sets up a new level when the Player
/// wants to play a game
/// </summary>
/// <param name="level">What level is to be loaded</param>
void Game::resetGame(int level)
{
	//Clear the target and wall vectors
	m_targets.clear();
	m_wallSprites.clear();
	//Reinitialise the vectors and the Tank
	m_Tank->resetPosition(m_Level[level - 1].m_tank.m_position);
	generateTargets(level);
	generateWalls(level);
	//Reset the statistic variables
	currentTarget = 0;
	m_hits = 0;
	m_shotsFired = 0;
	m_accuracy = 0;
	score = 0;
	m_countDownClock.restart(); // Reset the clock
}

/// <summary>
/// @brief Function simply reads from and writes to
/// a high score text file for storage
/// </summary>
void Game::fileReader()
{
	//Declare file streams
	std::ifstream inFile;
	std::ofstream outFile;
	//Set variables
	int scoreHigh = 0;
	int accuracyHigh = 0;
	m_highScore = 0;
	m_highAccuracy = 0;
	//Attempt to open text
	inFile.open("highscore.txt");
	//If the file doesn't exist
	if(inFile.fail())
	{
		inFile.close();
		outFile.open("highscore.txt"); // Make the file
		outFile << score << std::endl << m_accuracy;
	}
	else
	{
		inFile >> scoreHigh >> accuracyHigh; // Read the scores
		m_highScore = scoreHigh;
		m_highAccuracy = accuracyHigh;
		inFile.close();
		outFile.open("highscore.txt");
		if (score > scoreHigh) // Check if the scores have been beaten
		{
			scoreHigh = score;
		}
		if (m_accuracy > accuracyHigh)
		{
			accuracyHigh = m_accuracy;
		}
		outFile.clear(); // Clear the file
		outFile << scoreHigh << std::endl << accuracyHigh;// Rewrite the file
	}
}