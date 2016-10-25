#include "Tank.h"
#include <iostream>

double const Tank::DEG_TO_RAD = thor::Pi / 180.0f;

Tank::Tank(sf::Texture const & texture, sf::Vector2f const & pos)
: m_texture(texture)
{
	initSprites(pos);
}

void Tank::update(double dt)
{	
	if (true)
	{
		m_tankBase.setPosition(m_tankBase.getPosition().x + (std::cos(m_rotation * DEG_TO_RAD) * m_speed), m_tankBase.getPosition().y + (std::sin(m_rotation * DEG_TO_RAD) * m_speed));
		m_tankBase.setRotation(m_rotation);
		m_turret.setPosition(m_tankBase.getPosition().x + (std::cos(m_rotation * DEG_TO_RAD) * m_speed), m_tankBase.getPosition().y + (std::sin(m_rotation * DEG_TO_RAD) * m_speed));
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
	}
	std::cout << m_rotation;
	std::system("cls");
}

void Tank::render(sf::RenderWindow & window) 
{
	window.draw(m_tankBase);
	window.draw(m_turret);
}


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

void Tank::IncreaseSpeed()
{
	if (m_speed < 100)
	{
		m_speed += 0.1;
	}
}

void Tank::DecreaseSpeed()
{
	if (m_speed > -100)
	{
		m_speed -= 0.1;
	}
}

void Tank::IncreaseRotation()
{
	m_rotation += 1;
	if (m_rotation == 360.0)
	{
		m_rotation = 0.0;
	}
}

void Tank::DecreaseRotation()
{
	m_rotation -= 1;
	if (m_rotation == -360.0)
	{
		m_rotation = 0.0;
	}
}

void Tank::IncreaseTurretAngle()
{
	turretAddOn += 1;
	if (turretAddOn == 360)
	{
		turretAddOn = 0;
	}
}

void Tank::DecreaseTurretAngle()
{
	turretAddOn -= 1;
	if (turretAddOn == -360)
	{
		turretAddOn = 0;
	}
}