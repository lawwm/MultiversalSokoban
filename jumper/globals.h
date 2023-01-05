#ifndef GLOBALS_H
#define GLOBALS_H

#include <string>
#include <tuple>
#include <vector>
#include <unordered_map>

namespace sides {
	enum Side {
		TOP,
		BOTTOM,
		LEFT,
		RIGHT,
		NONE
	};

	inline Side getOppositeSide(Side side) {
		return
			side == TOP ? BOTTOM :
			side == BOTTOM ? TOP :
			side == LEFT ? RIGHT :
			side == RIGHT ? LEFT :
			NONE;
	}
}

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
	const std::string explode = "sprites/explode.png";
	const std::string endpoint = "sprites/2.png";
	const std::string dialogue = "sprites/dialogue-bubble.png";
	const std::string chest = "sprites/little-treasure-chest.png";
	
	const std::string opening_screen = "sprites/opening_screen.png";
	const std::string closing_screen = "sprites/closing_screen.png";
	
	const std::string a_room = "a_room";
	const std::string a_ice = "a_ice";

	const std::string b_room = "b_room";
	const std::string b_ice = "b_ice";

	const std::string c_room = "c_room";
	const std::string c_ice = "c_ice";

	const std::string d_room = "d_room";
	const std::string d_ice = "d_ice";

	const std::string e_room = "e_room";
	const std::string e_ice = "e_ice";

	const std::string iceLevel = "Ice1";
	const std::string roomLevel = "Room1";
	const std::string pierLevel = "Pier1";
	const std::string bushLevel = "Bush1";
	const std::string overworld = "overworld";
	
	const std::vector<zonedata> data = { // first element of each vector contains the element location details
		{{a_room, a_ice }},

		{{b_room, b_ice }},

		{{e_ice, e_room}},
		
		{{c_room, c_ice }},
		
		{{d_room, d_ice}}

		
	};

	const std::string died_dialogue = "died";
	const std::string won_dialogue = "won";
	const std::string exit_dialogue = "exit";
	const std::string overworld_exit_dialogue = "overworld_exit";

	const float overworld_spawn_x = 288;
	const float overworld_spawn_y = 192;
}

#endif
