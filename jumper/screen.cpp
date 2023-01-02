#include "screen.h"

OpeningScreen::OpeningScreen()
{
}

OpeningScreen::~OpeningScreen()
{
}

OpeningScreen::OpeningScreen(Graphics& graphics)
{
	TTF_Font* font = TTF_OpenFont("fonts/zx-spectrum.ttf", 11);
	SDL_Color white = { 255, 255, 255 };
	int texW = 0;
	int texH = 0;

	SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(font, "Press A to start", white, 504);
	SDL_Texture* message = SDL_CreateTextureFromSurface(graphics.getRenderer(), surfaceMessage);

	// find dimensions of the created texture, and assign it to output so it is not stretched
	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	std::cout << texW << " " << texH << std::endl;
	SDL_Rect message_rect{ 198, 380, texW, texH };

	_texts["start"] = std::make_pair(message, message_rect);
	

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

	SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(font, "You have mastered multiversal travel!\n         Congratulations (:", white, 504);
	SDL_Texture* message = SDL_CreateTextureFromSurface(graphics.getRenderer(), surfaceMessage);

	// find dimensions of the created texture, and assign it to output so it is not stretched
	SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
	std::cout << "victory" << texW << " " << texH << std::endl;
	SDL_Rect message_rect{ 60, 380, texW, texH };

	_texts["start"] = std::make_pair(message, message_rect);


	this->_background = Sprite(graphics, globals::opening_screen, 0, 0, 600, 480, 0, 0);
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
