#ifndef SPRITE_H
#define SPRITE_H

#include "SDL.h"

#include <string>

#include "rectangle.h"
#include "globals.h"

class Graphics;

/* Sprite class
 * Holds all information for individual sprites
 */

class Sprite {
public:
	Sprite();
	Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
		float posX, float posY);
	virtual ~Sprite();
	virtual void update();
	void draw(Graphics& graphics, int x, int y);
	void draw(Graphics& graphics, int x, int y, int width, int height);
	const Rectangle getBoundingBox() const;
	int getX() const;
	int getY() const;
protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;

	Rectangle _boundingBox;

	float _x, _y;	
};
#endif
