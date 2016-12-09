/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#include "Bullet.h"
 /// <summary>
 /// @brief Constructor function for the Bullet
 /// </summary>
 /// <param name="textureIn">bullet's texture</param>
Bullet::Bullet(sf::Texture & textureIn)
{
	m_sprite.setTexture(textureIn);
	sf::IntRect textureRect(8, 178, 8, 8);
	m_sprite.setTextureRect(textureRect);
	m_sprite.setOrigin(4, 4);
	m_sprite.setRotation(-90);
}
 /// <summary>
 /// @brief Function updates attributes of the bullet
 /// like position and aliv time and ultimately
 /// deactivates the bullet
 /// </summary>
 /// <param name="dt"></param>
 /// <param name="DEG_TO_RAD"></param>
void Bullet::Update(double dt, double DEG_TO_RAD)
{
	if (fired)
	{
		m_sprite.setPosition(m_sprite.getPosition().x + (std::cos(m_angle * DEG_TO_RAD) * SPEED * dt / 100), m_sprite.getPosition().y + (std::sin(m_angle * DEG_TO_RAD) * SPEED * dt / 100)); // Update the position
		aliveTime += dt; // Add on to alive time
		if (aliveTime > 5000) // We deactivate the bullet if the time goes above 5 seconds
		{
			fired = false;
			aliveTime = 0; // Rest alive time
		}
	}
}
 /// <summary>
 /// @brief Function for drawing te sprite
 /// </summary>
 /// <param name="window"></param>
void Bullet::Render(sf::RenderWindow & window)
{
	if (fired) // Only draw if active
	{
		window.draw(m_sprite);
	}
}
 /// <summary>
 /// @brief Function for initially firing the bullet by the tank
 /// </summary>
 /// <param name="angleIn">angle at which the bullet is fired</param>
 /// <param name="positionIn">initial position of the bullet</param>
void Bullet::Fire(float angleIn, sf::Vector2f positionIn)
{
	m_position = positionIn; // Set the position
	m_angle = angleIn; // Set the rotation
	m_sprite.setPosition(m_position);
	m_sprite.setRotation(m_angle - 90);
	fired = true;
}
 /// <summary>
 /// @brief Function simply deactivates the bullet
 /// </summary>
void Bullet::Collision()
{
		fired = false;
}