/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#include "Target.h"

/// <summary>
/// @brief Constructor for the target
/// </summary>
/// <param name="texture">target texture</param>
/// <param name="position">target position</param>
/// <param name="rotation">target rotation</param>
/// <param name="duration">target alive time</param>
/// <param name="offset">random offset</param>
/// <param name="score">score address</param>
/// <param name="currentTarget">current target address</param>
Target::Target(sf::Texture &texture, sf::Vector2f position, float rotation, double duration, float offset, int& score, int& currentTarget) : 
m_duration(duration),
m_alive(true),
m_offset(offset),
m_gameScore(score),
m_currentTarget(currentTarget)
{
	//Initialise the sprite
	m_sprite.setTexture(texture);
	m_sprite.setPosition(randPosition(position.x),randPosition(position.y));
	m_sprite.setRotation(rotation);
}

/// <summary>
/// @brief Destructor for the target
/// </summary>
Target::~Target(){}


/// <summary>
/// @brief Function updates all attributes of the sprite
/// </summary>
/// <param name="dt"></param>
void Target::update(double dt)
{
	if (m_alive) // Only update if the target is active
	{
		if (m_duration > 0.0)
		{
			m_duration -= dt; // Decrement the time left to live
		}
		else
		{
			m_alive = false; // Set alive to false if the target's time is up
			m_currentTarget++; // Chang the current Target
		}
		if (m_duration < 5000.0) // Flash if the target is about to be deactivated
		{
			if (static_cast<int>(m_duration / 250.0) % 2 != 0 && m_sprite.getColor().a < 246)
			{
				m_sprite.setColor(sf::Color(m_sprite.getColor().r, m_sprite.getColor().g, m_sprite.getColor().b, m_sprite.getColor().a + ALPHA_CHANGE)); // Increase the alpha 
			}
			else if (m_sprite.getColor().a > 10)
			{
				m_sprite.setColor(sf::Color(m_sprite.getColor().r, m_sprite.getColor().g, m_sprite.getColor().b, m_sprite.getColor().a - ALPHA_CHANGE)); // Decrease the alpha
			}
		}
	}
}

/// <summary>
/// @brief render function for the target to draw the sprite
/// </summary>
/// <param name="window"></param>
void Target::render(sf::RenderWindow & window)
{
	if (m_alive) // Only draw if alive
	{
		window.draw(m_sprite); //Draw call
	}
}

/// <summary>
/// @brief Function deactivates the target and increase the score
/// while incrementiung the current target
/// </summary>
void Target::hit()
{
	m_duration = 0;
	m_alive = false;
	m_gameScore += 5;
	m_currentTarget++;
}
 
/// <summary>
/// @brief Sprite getter function
/// </summary>
/// <returns>Traget sprite</returns>
sf::Sprite Target::getSprite()
{
	return m_sprite;
}

/// <summary>
/// @brief Getter Function for the m_alive bool
/// </summary>
/// <returns>bool active state</returns>
bool Target::getState()
{
	return m_alive;
}

/// <summary>
/// @brief Function randomises the position of the target
/// </summary>
/// <param name="posCoord">coordinates of the position to be offset</param>
/// <returns>randomised position</returns>
float Target::randPosition(float posCoord)
{
	std::time_t t = time(0); //Get the current time
	if (t % 2 == 0) //If the time is even add the offset
	{
		posCoord += m_offset;
	}
	else // Otherwise subtract it
	{
		posCoord -= m_offset;
	}
	return posCoord;
}