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
	const float SPRITE_WIDTH = 16.0f;
	
	// directory
	const std::string dir = "content/";
	
	// sound
	const std::string walk_sound = dir + "audio/walk-sound.wav";
	const std::string collide_sound = dir + "audio/collide-sound.wav";
	const std::string kill_sound = dir + "audio/kill-sound.wav";
	const std::string menu_sound = dir + "audio/menu-sound.wav";
	const std::string transition_sound = dir + "audio/transition-sound.wav";
	const std::string victory_sound = dir + "audio/victory-sound.wav";
	const std::string menu_close_sound = dir + "audio/menu-close-sound.wav";
	const std::string undo_sound = dir + "audio/undo-sound.wav";
	
	// music
	const std::string opening_music = dir + "audio/opening-music.ogg";
	const std::string game_music = dir + "audio/game-music.ogg";
	const std::string victory_music = dir + "audio/victory-music.ogg";
	
	// fonts
	const std::string minimal = dir + "fonts/Minimal5x7.ttf";
	
	// sprites
	
	const std::string eskimo = dir + "sprites/21.png";
	const std::string coin = dir + "sprites/coin-2.png";
	const std::string sushi = dir + "sprites/sushi.png";
	const std::string bomb = dir + "sprites/CanonBall.png";
	
	const std::string explode = dir + "sprites/explode.png";
	const std::string endpoint = dir + "sprites/2.png";
	const std::string dialogue = dir + "sprites/dialogue-bubble.png";
	const std::string chest = dir + "sprites/little-treasure-chest.png";
	
	
	// screens

	const std::string opening_screen = dir + "sprites/opening_screen.png";
	const std::string closing_screen = dir + "sprites/closing_screen.png";
	const std::string setting_screen = dir + "sprites/setting_screen.png";

	// maps

	const std::string a_yellow = dir + "a_yellow";
	const std::string a_ice = dir + "a_ice";

	const std::string b_yellow = dir + "b_yellow";
	const std::string b_ice = dir + "b_ice";

	const std::string c_room = dir + "c_room";
	const std::string c_ice = dir + "c_ice";

	const std::string d_green = dir + "d_green";
	const std::string d_yellow = dir + "d_yellow";

	const std::string e_yellow = dir + "e_yellow";
	const std::string e_green = dir + "e_green";

	const std::string f_room = dir + "f_room";
	const std::string f_ice = dir + "f_ice";

	const std::string g_yellow = dir + "g_yellow";

	const std::string h_yellow = dir + "h_yellow";
	const std::string h_green = dir + "h_green";

	const std::string i_yellow = dir + "i_yellow";
	const std::string i_ice = dir + "i_ice";

	const std::string j_yellow = dir + "j_yellow";
	const std::string j_ice = dir + "j_ice";

	const std::string k_ice = dir + "k_ice";
	const std::string k_yellow = dir + "k_yellow";

	const std::string l_ice = dir + "l_ice";
	const std::string l_yellow = dir + "l_yellow";
		
	const std::string m_ice = dir + "m_ice";
	const std::string m_yellow = dir + "m_yellow";
	
	const std::string n_ice = dir + "n_ice";
	const std::string n_yellow = dir + "n_yellow";

	const std::string overworld = dir + "overworld";
	
	const std::vector<zonedata> data = { // first element of each vector contains the element location details
		{{a_ice, a_yellow }}, // introduce player to dimension switching /done 1

		{{c_room, c_ice }},	// get the player to be better at dimension switching /done 2
		
		{{b_ice, b_yellow }}, // show an easy level that coins can get to the end /done 3
		
		{{e_green, e_yellow}}, // show an obvious path that cannot work /done 4

		{{d_green, d_yellow }}, // up the number of coins /done 5
		
		{{g_yellow}},	// introduce the spike tile g /done 6

		{{f_room, f_ice}},	// do a level with four dimensions // do this 7
		 		
		{{h_yellow, h_green }}, // more complicated level with spike tile /done 8
		
		{{j_ice, j_yellow }}, // introduce the sushi block j /done 9

		{{k_ice, k_yellow }}, // idk lmao 12 many many coins with sushi in the middle. Destroy the sushi by pushing the coins. /done 10

		{{i_ice, i_yellow }}, // level where you have to push blocks in as one line // done 11

		{{l_ice, l_yellow }}, // bomb level introduction // do this 12

		{{m_ice, m_yellow }}, // medium difficulty bomb level 14

		{{n_ice, n_yellow }}  // medium difficulty bomb level 14
	};

	const std::string died_dialogue = "died";
	const std::string won_dialogue = "won";
	const std::string exit_dialogue = "exit";
	const std::string opening_exit_dialogue = "opening_exit";
	const std::string overworld_exit_dialogue = "overworld_exit";
}

#endif
