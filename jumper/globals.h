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
	ZONE
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
	
	const std::string iceLevel = "Ice1";
	const std::string roomLevel = "Room1";
	const std::string pierLevel = "Pier1";
	const std::string bushLevel = "Bush1";
	const std::string overworld = "overworld";
	
	const std::vector<zonedata> data = {
		{{iceLevel, roomLevel }, Vector2(160, 160), Vector2(160, 192), {{128,128}, {160,128}, {224,128}, {192,128}} },
		{{pierLevel, bushLevel}, Vector2(160, 160), Vector2(224, 224), {{224,128}, {192,128}} }
	};

	const std::unordered_map<int, int> overworldstages = {
		{96064, 0},
		{57728, 1},
		{19392, 2},
		{19584, 3},
		{58048, 4},
		{96512, 5},
		{173312, 6},
		{211648, 7},
		{249984, 8},
		{249792, 9},
		{211328, 10},
		{172864, 11}
	};

	const std::string died_dialogue = "died";
	const std::string won_dialogue = "won";
	const std::string exit_dialogue = "exit";
	//std::unordered_map<std::string, std::string> dialogueData({
	//	{died_dialogue, "You lost. Press Z to undo or R to restart"},
	//	{won_dialogue, "You won. Press N to proceed."},
	//	{exit_dialogue, "Press Esc again to confirm exit.\nPress A to continue playing."},
	//	{"1", "Press A to proceed to Stage 1"}
	//});
}

#endif
