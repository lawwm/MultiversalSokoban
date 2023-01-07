#include "sprite.h"

Sprite::Sprite() {}

Sprite::Sprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY) :
	_x(posX),
	_y(posY),
	_boundingBox(Rectangle(posX, posY, width, height))
{
	this->_sourceRect.x = sourceX;
	this->_sourceRect.y = sourceY;
	this->_sourceRect.w = width;
	this->_sourceRect.h = height;

	this->_spriteSheet = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(filePath));
	if (this->_spriteSheet == nullptr) {
		printf("\nError: Unable to load image\n");
	}
}

Sprite::~Sprite() {}

void Sprite::draw(Graphics& graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * globals::SPRITE_SCALE,
			this->_sourceRect.h * globals::SPRITE_SCALE };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::draw(Graphics& graphics, int x, int y, int width, int height) {
	SDL_Rect destinationRectangle = { x, y, width * globals::SPRITE_SCALE,
			height* globals::SPRITE_SCALE };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y,
		this->_sourceRect.w * globals::SPRITE_SCALE, this->_sourceRect.h * globals::SPRITE_SCALE);
}

const Rectangle Sprite::getBoundingBox() const {
	return this->_boundingBox;
}

int Sprite::getX() const {
	return this->_x;
}

int Sprite::getY() const {
	return this->_y;
}

bool Sprite::isStationary() const {
	int intx = floor(this->_x);
	int inty = floor(this->_y);
	bool isCloseToInt = std::abs(this->_x - floor(this->_x)) < 0.000000001f && std::abs(this->_y - floor(this->_y)) < 0.000000001f;
	int spritesize = floor(globals::SPRITE_SCALE * globals::SPRITE_WIDTH);
	return !(intx % spritesize) && !(inty % spritesize) && isCloseToInt;
}