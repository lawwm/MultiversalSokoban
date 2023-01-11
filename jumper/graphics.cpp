#include "graphics.h"


#include "SDL.h"
/* Graphics class
 * Holds all information dealing with graphics for the game
 */

Graphics::Graphics() {
	this->_window = SDL_CreateWindow("Sokoban of the Multiverse", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, Window::getScreenWidth(), Window::getScreenHeight(), Window::getScreenSetting());
	SDL_SetWindowBordered(this->_window, SDL_TRUE);

	
	this->_renderer = SDL_CreateRenderer(this->_window, -1, SDL_RENDERER_ACCELERATED);

	TTF_Init(); // might need to move this to game.cpp
	
	SDL_Surface* icon = IMG_Load("sprites/icon.png");
	
	SDL_SetWindowTitle(this->_window, "Sokoban of the Multiverse");
	SDL_SetWindowIcon(this->_window, icon);
}

Graphics::~Graphics() {
	TTF_Quit();
	SDL_DestroyWindow(this->_window);
	SDL_DestroyRenderer(this->_renderer);
}

SDL_Surface* Graphics::loadImage(const std::string& filePath) {
	if (this->_spriteSheets.count(filePath) == 0) {
		this->_spriteSheets[filePath] = IMG_Load(filePath.c_str());
	}
	return this->_spriteSheets[filePath];
}

void Graphics::blitSurface(SDL_Texture* texture, SDL_Rect* sourceRectangle, SDL_Rect* destinationRectangle) {
	SDL_RenderCopy(this->_renderer, texture, sourceRectangle, destinationRectangle);
}

void Graphics::flip() {
	SDL_RenderPresent(this->_renderer);
}

void Graphics::clear() {
	SDL_RenderClear(this->_renderer);
}

SDL_Renderer* Graphics::getRenderer() const {
	return this->_renderer;
}
