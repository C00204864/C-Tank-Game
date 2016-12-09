/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#include "Tank.h"

//constants
double const Tank::DEG_TO_RAD = thor::Pi / 180.0f;
int const Tank::NUMBER_OF_BULLETS = 5;
//vector
std::vector<Bullet> Tank::m_bulletVector;

/// <summary>
/// @brief Tank constructor function
/// </summary>
/// <param name="texture"></param>
/// <param name="pos"></param>
/// <param name="keyHandlerIn"></param>
/// <param name="soundIn"></param>
/// <param name="pIn"></param>
Tank::Tank(sf::Texture & texture, sf::Vector2f const & pos, KeyHandler & keyHandlerIn, sf::Sound&  soundIn, int * pIn)
	: m_texture(texture), 
	m_keyHandler(keyHandlerIn),
	m_gunShotSound(soundIn),
	m_pShotsTaken(pIn)
{
	initSprites(randPosition(pos));
	for (int i = 0; i < NUMBER_OF_BULLETS; i++)
	{
		m_bulletVector.push_back(Bullet(texture));
	}
}

/// <summary>
/// @brief Tank Destructor function
/// </summary>
Tank::~Tank(){}

/// <summary>
/// @brief Function updates the position of the tank
/// </summary>
/// <param name="dt">delta time(time since last update)</param>
/// <param name="collision">collision bool</param>
void Tank::update(double dt, bool collision)
{	
	if (!collision)
	{
		m_lastTankPosition = m_tankBase.getPosition();
		m_lastTurretPosition = m_turret.getPosition();
		m_lastRotation = m_rotation;
		m_lastTurretAddOn = turretAddOn;
	}
	HandleKeyInput();
	m_tankBase.setPosition(m_tankBase.getPosition().x + (std::cos(m_rotation * DEG_TO_RAD) * m_speed * dt / 50), m_tankBase.getPosition().y + (std::sin(m_rotation * DEG_TO_RAD) * m_speed * dt / 50));
	m_tankBase.setRotation(m_rotation);
	m_turret.setPosition(m_tankBase.getPosition());		
	int turretRotation = m_rotation + turretAddOn;
	if (turretRotation >= 360)
	{
		turretRotation -= 360;
	}
	else if(turretRotation <= -360)
	{
		turretRotation += 360;
	}
	m_turret.setRotation(turretRotation);
	if (collision)
	{
		m_rotation = m_lastRotation;
		turretAddOn = m_lastTurretAddOn;
		turretRotation = m_rotation + turretAddOn;
		m_tankBase.setRotation(m_rotation);
		m_turret.setRotation(turretRotation);
		m_tankBase.setPosition(m_lastTankPosition);
		m_turret.setPosition(m_lastTurretPosition);
		m_speed = 0;
	}

	//do not let the tank go too far off screen
	if (m_tankBase.getPosition().x > 1500)
	{
		m_tankBase.setPosition(-50, m_tankBase.getPosition().y);
		m_turret.setPosition(m_tankBase.getPosition());
	}
	else if (m_tankBase.getPosition().x < -100)
	{
		m_tankBase.setPosition(1450, m_tankBase.getPosition().y);
		m_turret.setPosition(m_tankBase.getPosition());
	}
	if (m_tankBase.getPosition().y > 1000)
	{
		m_tankBase.setPosition(m_tankBase.getPosition().x, -50);
		m_turret.setPosition(m_tankBase.getPosition());
	}
	else if (m_tankBase.getPosition().y < -100)
	{
		m_tankBase.setPosition(m_tankBase.getPosition().x, 950);
		m_turret.setPosition(m_tankBase.getPosition());
	}
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		m_bulletVector.at(i).Update(dt, static_cast<double>(DEG_TO_RAD));
	}
	timeSinceLastShot += dt;
}

/// <summary>
/// @brief Function updates the tank's attributes
/// based on key input
/// </summary>
void Tank::HandleKeyInput()
{
	if (m_keyHandler.isPressed(sf::Keyboard::Up))
	{
		IncreaseSpeed();
	}
	else if (m_keyHandler.isPressed(sf::Keyboard::Down))
	{
		DecreaseSpeed();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Left))
	{
		DecreaseRotation();
	}
	else if (m_keyHandler.isPressed(sf::Keyboard::Right))
	{
		IncreaseRotation();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::A))
	{
		DecreaseTurretAngle();
	}
	else if (m_keyHandler.isPressed(sf::Keyboard::D))
	{
		IncreaseTurretAngle();
	}
	if (m_keyHandler.isPressed(sf::Keyboard::Space) && timeSinceLastShot > 1000)
	{
		Shoot();
		timeSinceLastShot = 0;
	}
}

/// <summary>
/// @brief Function draws the tank's sprites
/// as well as calls the draw function for
/// the bullets
/// </summary>
/// <param name="window"></param>
void Tank::render(sf::RenderWindow & window) 
{
	for (int i = 0; i < m_bulletVector.size(); i++)
	{
		m_bulletVector.at(i).Render(window);
	}
	window.draw(m_tankBase);
	window.draw(m_turret);
}

/// <summary>
/// @brief Function initialises and sets 
/// the position of the two sprites for
/// the tank
/// </summary>
/// <param name="pos">position of the sprites</param>
void Tank::initSprites(sf::Vector2f const & pos)
{
	// Initialise the tank base
	m_tankBase.setTexture(m_texture);
	sf::IntRect baseRect(2, 43, 79, 43);
	m_tankBase.setTextureRect(baseRect);
	m_tankBase.setOrigin(baseRect.width / 2.0, baseRect.height / 2.0);
	m_tankBase.setPosition(pos);

	// Initialise the turret
	m_turret.setTexture(m_texture);
	sf::IntRect turretRect(19, 1, 83, 31);
	m_turret.setTextureRect(turretRect);
	m_turret.setOrigin(turretRect.width / 3.0, turretRect.height / 2.0);
	m_turret.setPosition(pos);
}

/// <summary>
/// @brief Function simply inreases the
/// tank's speed
/// </summary>
void Tank::IncreaseSpeed()
{
	if (m_speed < 20) //Don't let speed go above 20
	{
		m_speed += 0.1;
	}
}

/// <summary>
/// @brief Function simply decreases the
/// tank's speed
/// </summary>
void Tank::DecreaseSpeed()
{
	if (m_speed > -20) //Don't let speed go below -20
	{
		m_speed -= 0.1;
	}
}

/// <summary>
/// @brief Function simply increases the
/// rotation of the tank
/// </summary>
void Tank::IncreaseRotation()
{
	m_rotation += 1;
	if (m_rotation == 360.0) //360 == 0
	{
		m_rotation = 0.0;
	}
}

/// <summary>
/// @brief Function simply decreases the 
/// rotation of the tank
/// </summary>
void Tank::DecreaseRotation()
{
	m_rotation -= 1;
	if (m_rotation == -360.0) //-360 == 0
	{
		m_rotation = 0.0;
	}
}

/// <summary>
/// @brief Function inreases the angle in 
/// which the turret of the tank is facing 
/// </summary>
void Tank::IncreaseTurretAngle()
{
	turretAddOn += 1;
	if (turretAddOn == 360) //360 == 0
	{
		turretAddOn = 0;
	}
}

/// <summary>
/// @brief Function decreases the angle in 
/// which the turret of the tank is facing
/// </summary>
void Tank::DecreaseTurretAngle()
{
	turretAddOn -= 1;
	if (turretAddOn == -360) //-360 == 0
	{
		turretAddOn = 0;
	}
}

/// <summary>
/// @brief Getter method for the tank's sprite
/// </summary>
/// <returns>the tank's sprite</returns>
sf::Sprite Tank::GetTurretSprite()
{
	return m_tankBase;
}

/// <summary>
/// @brief Function allows the tank to shot by 
/// choosing an availible bullet from the bullet pool
/// </summary>
void Tank::Shoot()
{
	(*m_pShotsTaken)++; //Increment the dereferenced shots taken
	m_gunShotSound.play(); //Play Sound
	//Search for an availible bullet
	if (!m_bulletVector.at(0).fired)
	{
		m_bulletVector.at(0).Fire((m_rotation + turretAddOn), m_turret.getPosition());
	}
	else if (!m_bulletVector.at(1).fired)
	{
		m_bulletVector.at(1).Fire((m_rotation + turretAddOn), m_turret.getPosition());
	}
	else if (!m_bulletVector.at(2).fired)
	{
		m_bulletVector.at(2).Fire((m_rotation + turretAddOn), m_turret.getPosition());
	}
	else if (!m_bulletVector.at(3).fired)
	{
		m_bulletVector.at(3).Fire((m_rotation + turretAddOn), m_turret.getPosition());
	}
	else if (!m_bulletVector.at(4).fired)
	{
		m_bulletVector.at(4).Fire((m_rotation + turretAddOn), m_turret.getPosition());
	}
}

/// <summary>
/// @brief Function siply randomises which corner the tank will start in
/// </summary>
/// <param name="pos">central position of the tank</param>
/// <returns>new randomised position</returns>
sf::Vector2f Tank::randPosition(sf::Vector2f pos)
{
	srand(time(0)); //Seed the random generation
	if (rand() % 2 == 0)
	{
		pos.x += 650;
	}
	else
	{
		pos.x -= 650;
	}
	if (rand() % 2 == 0)
	{
		pos.y += 400;
	}
	else 
	{
		pos.y -= 400;
	}
	return pos;
}

/// <summary>
/// @brief Function simply resets and randomises the tanks position without 
/// creating a new instance of Tank
/// </summary>
/// <param name="pos">central position of the tank</param>
void Tank::resetPosition(sf::Vector2f pos)
{
	m_tankBase.setPosition(randPosition(pos));
	m_turret.setPosition(m_tankBase.getPosition());
	m_rotation = 0;
	turretAddOn = 0;
	m_speed = 0;
}