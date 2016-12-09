/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#pragma once
#include <SFML\Graphics.hpp>

/// <summary>
/// @brief Bullet class used by the tank to hit the targets
/// </summary>
class Bullet {
public:
	//Functions and Objects and Variables
	Bullet(sf::Texture & textureIn);
	void Update(double dt, double DEG_TO_RAD);
	void Render(sf::RenderWindow & window);
	void Fire(float angleIn, sf::Vector2f positionIn);
	void Collision();
	sf::Sprite m_sprite;
	bool fired = false;
private:
	//Objects and variables
	sf::Vector2f m_position;
	sf::Vector2f m_velocity;
	float m_angle;
	const int SPEED = 100;
	int aliveTime = 0;
};