#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

enum Screen {
	OVERWORLD,
	ZONE,
	START_SCREEN,
	VICTORY_SCREEN
};

enum Direction {
	LEFT,
	RIGHT,
	UP,
	DOWN
};

enum inLevelSelect {
	YES,
	NO,
	ESC
};

struct Vector2 {
	int x, y;
	Vector2() :
		x(0), y(0)
	{}
	Vector2(int x, int y) :
		x(x), y(y)
	{}
	Vector2 zero() {
		return Vector2(0, 0);
	}
};

typedef std::tuple<std::vector<std::string>> zonedata; // paths to map, spawn coordinates, end points, coordinates of moveables, 

namespace globals {
	const int SCREEN_WIDTH = 600;
	const int SCREEN_HEIGHT = 480;

	const float SPRITE_SCALE = 2.0f;
	const float SPRITE_WIDTH = 16.0f;
	
	const std::string eskimo = "sprites/21.png";
	const std::string coin = "sprites/coin-2.png";
	const std::string sushi = "sprites/sushi.png";
	const std::string explode = "sprites/explode.png";
	const std::string endpoint = "sprites/2.png";
	const std::string dialogue = "sprites/dialogue-bubble.png";
	const std::string chest = "sprites/little-treasure-chest.png";
	
	const std::string opening_screen = "sprites/opening_screen.png";
	const std::string closing_screen = "sprites/closing_screen.png";
	
	const std::string a_yellow = "a_yellow";
	const std::string a_ice = "a_ice";

	const std::string b_yellow = "b_yellow";
	const std::string b_ice = "b_ice";

	const std::string c_room = "c_room";
	const std::string c_ice = "c_ice";

	const std::string d_green = "d_green";
	const std::string d_yellow = "d_yellow";

	const std::string e_yellow = "e_yellow";
	const std::string e_green = "e_green";

	const std::string f_room = "f_room";
	const std::string f_ice = "f_ice";

	const std::string g_room = "g_room";
	const std::string g_ice = "g_ice";

	const std::string h_yellow = "h_yellow";
	const std::string h_ice = "h_ice";

	const std::string i_yellow = "i_yellow";
	const std::string i_ice = "i_ice";

	const std::string j_yellow = "j_yellow";
	const std::string j_ice = "j_ice";

	const std::string k_ice = "k_ice";
	const std::string k_yellow = "k_yellow";

	const std::string l_ice = "l_ice";
	const std::string l_yellow = "l_yellow";

	const std::string overworld = "overworld";
	
	const std::vector<zonedata> data = { // first element of each vector contains the element location details
		{{a_ice, a_yellow }}, // introduce player to dimension switching

		{{b_ice, b_yellow }}, // get the player to be better at dimension switching

		{{c_room, c_ice }},	// show an easy level that coins can get to the end
		
		{{d_green, d_yellow }}, // up the number of coins

		{{e_green, e_yellow}}, // show an obvious path that cannot work
		
		{{f_room, f_ice}},	// do a harder level f
		 
		{{g_room, g_ice}},	// introduce the spike tile g 
		
		{{h_yellow, h_ice }}, // level where you have to push blocks in as one line h

		{{i_yellow, i_ice }}, // level exploiting the trick i
	
		{{j_ice, j_yellow }}, // introduce the sushi block j

		{{k_ice, k_yellow }},	// level where you have to push blocks in as one line but with sushi block

		{{l_ice, l_yellow }} // idk lmao
	};

	const std::string died_dialogue = "died";
	const std::string won_dialogue = "won";
	const std::string exit_dialogue = "exit";
	const std::string overworld_exit_dialogue = "overworld_exit";

	const float overworld_spawn_x = 288;
	const float overworld_spawn_y = 192;
}

#endif
