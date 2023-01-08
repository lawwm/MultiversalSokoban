#include "screen.h"

OpeningScreen::OpeningScreen()
{
}

OpeningScreen::~OpeningScreen()
{
}

OpeningScreen::OpeningScreen(Graphics& graphics)
{
	SDL_Color white = { 255, 255, 255 };
	int texW = 0;
	int texH = 0;

	this->_background = Sprite(graphics, globals::opening_screen, 0, 0, 600, 480, 0, 0);
}

void OpeningScreen::draw(Graphics& graphics)
{
	this->_background.draw(graphics, 0, 0, 300, 240);
	for (auto& [key, value] : this->_texts) {
		graphics.blitSurface(value.first, nullptr, &value.second);
	}
}

void OpeningScreen::update(float elapsedTime, Graphics& graphics)
{
	this->_background.update();
}


VictoryScreen::VictoryScreen()
{
}

VictoryScreen::~VictoryScreen()
{
}

VictoryScreen::VictoryScreen(Graphics& graphics)
{
	TTF_Font* font = TTF_OpenFont("fonts/zx-spectrum.ttf", 11);
	SDL_Color white = { 255, 255, 255 };
	int texW = 0;
	int texH = 0;

	this->_background = Sprite(graphics, globals::closing_screen, 0, 0, 600, 480, 0, 0);
}

void VictoryScreen::draw(Graphics& graphics)
{
	this->_background.draw(graphics, 0, 0, 300, 240);
	for (auto& [key, value] : this->_texts) {
		graphics.blitSurface(value.first, nullptr, &value.second);
	}
}

void VictoryScreen::update(float elapsedTime, Graphics& graphics)
{
	this->_background.update();
}
