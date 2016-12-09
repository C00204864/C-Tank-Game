/// <summary>
/// @author Liam Hickey
/// @date December 2017
/// @version 2.0
/// </summary>

#include "MenuLoader.h"

//Default constructor
MenuLoader::MenuLoader(){}

//Operator Overloads
void operator >> (YAML::Node & node, Background & background)
{
	background.m_filename = node["file"].as<std::string>();
	return;
}

void operator >> (YAML::Node & node, Font & font)
{
	font.m_filename = node["file"].as<std::string>();
}

void operator >> (YAML::Node & node, Text & text)
{
	text.m_text = node["string"].as<std::string>();
	text.m_position.x = node["positionx"].as<float>();
	text.m_position.y = node["positiony"].as<float>();
	text.m_state = node["boolstate"].as<bool>();
}

void operator >> (YAML::Node & node, Heading & heading)
{
	heading.m_text = node["string"].as<std::string>();
	heading.m_position.x = node["positionx"].as<float>();
	heading.m_position.y = node["positiony"].as<float>();
}

void operator >> (YAML::Node & node, LevelsText & level)
{
	level.m_text = node["string"].as<std::string>();
	level.m_position.x = node["positionx"].as<float>();
	level.m_position.y = node["positiony"].as<float>();
	level.m_state = node["boolstate"].as<bool>();
}

/// <summary>
/// @brief Function loads all of the attributes from the YAML file
/// into the object of type MenuLoader
/// </sumary>
bool MenuLoader::load()
{
	//Try to catch exceptions when reading files
	try 
	{
		//Nodes to variables
		YAML::Node baseNode = YAML::LoadFile(m_filePath);
		if (baseNode.IsNull())
		{
			std::string message("file: " + m_filePath + " not found");
			throw std::exception(message.c_str());
		}

		baseNode["background"] >> m_background;
		baseNode["font"] >> m_font;
		YAML::Node & textNode = baseNode["text"].as<YAML::Node>();
		Text textHolder;
		for (int i = 0; i < textNode.size(); i++)
		{
			textNode[i] >> textHolder;
			m_textArray.push_back(textHolder);
		}
		baseNode["heading"] >> m_heading;
		YAML::Node & levelsNode = baseNode["levelstext"].as<YAML::Node>();
		LevelsText levelHolder;
		for (int i = 0; i < levelsNode.size(); i++)
		{
			levelsNode[i] >> levelHolder;
			m_levelsTextArray.push_back(levelHolder);
		}
	}

	//Exceptions
	catch (YAML::ParserException & exception)
	{
		std::cout << exception.what() << std::endl;
		return false;
	}
	catch (std::exception & exception)
	{
		std::cout << exception.what() << std::endl;
		return false;
	}

	//Return true if the load was successful
	return true;
}