/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#pragma once
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <Thor/Math.hpp>
#include "KeyHandler.h"
#include <vector>
#include <ctime>
#include "Bullet.h"

/// <summary>
/// @brief A simple tank controller.
/// This class will manage all tank movement and rotations.
/// </summary>
class Tank
{
public:	
	//Function Declarations
	Tank(sf::Texture & texture, sf::Vector2f const & pos, KeyHandler & keyHandlerIn, sf::Sound &, int *);
	~Tank();
	void update(double dt, bool);
	void HandleKeyInput();
	void render(sf::RenderWindow & window);
	void IncreaseSpeed();
	void DecreaseSpeed();
	void IncreaseRotation();
	void DecreaseRotation();
	sf::Sprite GetTurretSprite();
	void IncreaseTurretAngle();
	void DecreaseTurretAngle();
	static std::vector<Bullet> m_bulletVector;
	void resetPosition(sf::Vector2f);

	//Constant
	int static const NUMBER_OF_BULLETS;

private:
	//Function Declarations
	sf::Vector2f randPosition(sf::Vector2f);
	void initSprites(sf::Vector2f const & pos);
	void Shoot();

	//Objects and variables
	//Tank base sprite
	sf::Sprite m_tankBase;
	//Tank turret sprite
	sf::Sprite m_turret;
	//Sprite sheet texture
	sf::Texture const & m_texture;
	//Tank's speed
	double m_speed = 0;
	//Tank's rotation
	double m_rotation = 0;
	//Tan's last rotation
	double m_lastRotation;
	//Tank's last turret add on
	double m_lastTurretAddOn;
	//CONSTANT degree/radian converter
	double static const DEG_TO_RAD;
	//Turret add on
	int turretAddOn = 0;
	//Key handler
	KeyHandler & m_keyHandler;
	//Last Tank position(last frame)
	sf::Vector2f m_lastTankPosition;
	//Last turret position (last frame)
	sf::Vector2f m_lastTurretPosition;
	//Time since the last successful shot
	float timeSinceLastShot = 0;
	//Shooting sound
	sf::Sound m_gunShotSound;
	//Pointer to shots taken in Game
	int * m_pShotsTaken;
};
