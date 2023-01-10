#ifndef WINDOWSIZE_H
#define WINDOWSIZE_H

#include <vector>
#include <tuple>
#include "SDL.h"

class Window {
public:
	
	static float getSpriteScale();
	
	static float getPlayerSpeed();
	
	static int getScreenWidth();
	
	static int getScreenHeight();
	
	static int getTextSize();
	
	static Uint32 getScreenSetting();

	static void setWindowSize(int idx);

private:
	static std::vector<std::tuple<float, float, int, int, int, Uint32>> _windowSettings; // sprite scale, player speed, screen width, screen height, text size 
	static int idx;
};
#endif