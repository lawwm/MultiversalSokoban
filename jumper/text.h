#ifndef TEXT_H
#define TEXT_H

#include "SDL_ttf.h"

#include "graphics.h"
#include "globals.h";
#include <vector>
#include <unordered_map>

class TextBox {
public:
	TextBox() {}

	TextBox(Graphics& graphics, std::unordered_map<std::string, std::string> texts) {
		TTF_Font* font = TTF_OpenFont("fonts/zx-spectrum.ttf", 11);
		SDL_Color white = { 0, 0, 0 };
		int texW = 0;
		int texH = 0;
		for (auto [key, value] : texts) {
			// Create texture from the text
			SDL_Surface* surfaceMessage = TTF_RenderText_Blended_Wrapped(font, value.c_str(), white, 504);
			SDL_Texture* message = SDL_CreateTextureFromSurface(graphics.getRenderer(), surfaceMessage);
			
			// find dimensions of the created texture, and assign it to output so it is not stretched
			SDL_QueryTexture(message, NULL, NULL, &texW, &texH);
			SDL_Rect message_rect{ 69, 357, texW, texH };

			_texts[key] = std::make_pair(message, message_rect);
		}
		
		this->_dialogue = Sprite(graphics, globals::dialogue, 0, 0, 316, 60, 0, 0);
		TTF_CloseFont(font);
	}

	~TextBox() {
		
	}

	void set(std::string key) {
		this->_currKey = key;
	}

	std::string getKey() {
		return this->_currKey;
	}

	void draw(Graphics& graphics) {
		if (_currKey == "") {
			return;
		}
		this->_dialogue.draw(graphics, 48, 336, 252, 48); // x offset, y offset, width, height
		graphics.blitSurface(_texts[_currKey].first, NULL, &_texts[_currKey].second);
	}

private:
	std::unordered_map<std::string, std::pair<SDL_Texture*, SDL_Rect>> _texts;
	std::string _currKey = "";
	Sprite _dialogue;
};

#endif
