/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#pragma once

#include <SFML/System/Vector2.hpp>
#include <vector>
#include <sstream>
#include <fstream>
#include <iostream>
#include "yaml-cpp\yaml.h"


//Component Structs
struct Background {
	std::string m_filename;
};

struct Font {
	std::string m_filename;
};

struct Text {
	std::string m_text;
	sf::Vector2f m_position;
	bool m_state;
};

struct Heading {
	std::string m_text;
	sf::Vector2f m_position;
};

struct LevelsText {
	std::string m_text;
	sf::Vector2f m_position;
	bool m_state;
};

//MenuLoader CLlass with incorporated structs
class MenuLoader {
public:
	MenuLoader();
	bool load();
	Background m_background;
	Font m_font;
	Heading m_heading;
	std::vector<Text> m_textArray;
	std::vector<LevelsText> m_levelsTextArray;
private:
	std::string m_filePath = "resources\\levels\\menu.yaml";
};