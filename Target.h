/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#pragma once
#include <SFML\Graphics.hpp>
#include <ctime>

/// <summary>
/// @brief Target class is the used to activate and deactivate targets
/// </summary>
class Target {
public:
	Target(sf::Texture &, sf::Vector2f, float, double, float, int&, int&);
	~Target();
	void update(double);
	void render(sf::RenderWindow &);
	sf::Sprite getSprite();
	bool getState();
	void hit();
private:
	float randPosition(float);
	//Traget's sprite
	sf::Sprite m_sprite;
	//When the target disappears
	double m_duration;
	//Whether to draw the target or not
	bool m_draw = true;
	//Traget active bool
	bool m_alive;
	//offset used by the random generator
	float m_offset;
	//refernce to the gameScore
	int & m_gameScore;
	//refernce to the current target
	int & m_currentTarget;
	//Alpha changer
	const int ALPHA_CHANGE = 8;
};
