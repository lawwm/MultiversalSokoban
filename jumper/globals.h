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

typedef std::tuple<std::vector<std::string>, Vector2, Vector2, std::vector<Vector2>> zonedata; // paths to map, spawn coordinates, end points, coordinates of moveables, 

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

	
	const std::string a_room = "a_room";
	const std::string a_ice = "a_ice";

	const std::string b_room = "b_room";
	const std::string b_ice = "b_ice";

	const std::string iceLevel = "Ice1";
	const std::string roomLevel = "Room1";
	const std::string pierLevel = "Pier1";
	const std::string bushLevel = "Bush1";
	const std::string overworld = "overworld";
	
	const std::vector<zonedata> data = {
		{{a_room, a_ice }, Vector2(128, 128), Vector2(480, 128), {{160,96}, {160, 128}, {160,  160}} },
		{{b_room, b_ice }, Vector2(160, 160), Vector2(448, 224), {{224,160}, {352,160}, {384,160}, {384, 192}, {384, 224}, {352, 224}, {416, 256}, {416, 288}, {416, 128}} },
		{{iceLevel, roomLevel }, Vector2(160, 160), Vector2(160, 192), {{128,128}, {160,128}, {224,128}, {192,128}} },
		{{pierLevel, bushLevel}, Vector2(160, 160), Vector2(224, 224), {{224,128}, {192,128}} }
	};

	const std::string died_dialogue = "died";
	const std::string won_dialogue = "won";
	const std::string exit_dialogue = "exit";
	const std::string overworld_exit_dialogue = "overworld_exit";

	const float overworld_spawn_x = 288;
	const float overworld_spawn_y = 192;
}

#endif
