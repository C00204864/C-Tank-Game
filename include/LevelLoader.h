#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"

struct ObstacleData
{
	std::string m_type;
	sf::Vector2f m_position;
	double m_rotation;
};

struct BackgroundData
{
	std::string m_fileName;
};

struct TankData
{
	sf::Vector2f m_position;
};

struct TargetData
{
	sf::Vector2f m_position;
	float m_rotation;
	float m_duration;
	float m_offset;
};

struct TargetFile
{
	std::string m_filename;
};

struct ResetString
{
	std::string m_string;
};

struct LevelData
{
	BackgroundData m_background;
    TankData m_tank;
    std::vector<ObstacleData> m_obstacles;
	std::vector<TargetData> m_targets;
	TargetFile m_targetFile;
	ResetString m_resetString;
};

class LevelLoader
{
public:
   LevelLoader();
   static bool load(int nr, LevelData& level);
};