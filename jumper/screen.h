#ifndef SCREEN_H
#define SCREEN_H

#include "SDL.h"
#include "SDL_ttf.h"

#include "sprite.h"
#include "globals.h"
#include "graphics.h"

#include <unordered_map>
#include <iostream>

class OpeningScreen {
public:
	OpeningScreen();
	~OpeningScreen();

	OpeningScreen(Graphics& graphics);

	void draw(Graphics& graphics);
	void update(float elapsedTime, Graphics& graphics);

private:
	std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect>> _texts;
	Sprite _background;
};

class VictoryScreen {
public:
	VictoryScreen();
	~VictoryScreen();

	VictoryScreen(Graphics& graphics);

	void draw(Graphics& graphics);
	void update(float elapsedTime, Graphics& graphics);

private:
	std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect>> _texts;
	Sprite _background;
};

#endif