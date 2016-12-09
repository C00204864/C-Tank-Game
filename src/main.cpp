/// <summary>
/// @mainpage Tank Battler - 2d vector based top down shooter
/// @Author Liam Hickey / Ross Palmer
/// @Version 2.0
/// @brief Programming Project One - Tank Game
///
/// A game where you control a tank using the arrow buttons as well
/// as A and D with Space in order to shoot targets within a time
/// limit and try and beat the stored high score
/// 
/// Total Time Taken 20hrs
/// </summary>


#ifdef _DEBUG 
#pragma comment(lib,"sfml-graphics-d.lib") 
#pragma comment(lib,"sfml-audio-d.lib") 
#pragma comment(lib,"sfml-system-d.lib") 
#pragma comment(lib,"sfml-window-d.lib") 
#pragma comment(lib,"sfml-network-d.lib") 
#pragma comment(lib,"thor-d.lib")
#else 
#pragma comment(lib,"sfml-graphics.lib") 
#pragma comment(lib,"sfml-audio.lib") 
#pragma comment(lib,"sfml-system.lib") 
#pragma comment(lib,"sfml-window.lib") 
#pragma comment(lib,"sfml-network.lib") 
#pragma comment(lib,"thor.lib")
#endif 
#pragma comment(lib,"opengl32.lib") 
#pragma comment(lib,"glu32.lib") 

//YAML add-on------------------------
#pragma comment(lib,"libyaml-cppmdd")

#include "Game.h"

/// <summary>
/// @brief starting point for all C++ programs.
/// Create a game object and run it.
/// </summary>
/// <param name=""></param>
/// <param name="argv"></param>
/// <returns></returns>
int main(int, char* argv[])
{
	Game game;
	game.run();
	rand();
}