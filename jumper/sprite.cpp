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
	SDL_Rect destinationRectangle = { x, y, this->_sourceRect.w * Window::getSpriteScale(),
			this->_sourceRect.h * Window::getSpriteScale() };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::draw(Graphics& graphics, int x, int y, int width, int height) {
	SDL_Rect destinationRectangle = { x, y, width * Window::getSpriteScale(),
			height* Window::getSpriteScale() };
	graphics.blitSurface(this->_spriteSheet, &this->_sourceRect, &destinationRectangle);
}

void Sprite::update() {
	this->_boundingBox = Rectangle(this->_x, this->_y,
		this->_sourceRect.w * Window::getSpriteScale(), this->_sourceRect.h * Window::getSpriteScale());
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
	int spritesize = floor(Window::getSpriteScale() * globals::SPRITE_WIDTH);
	return !(intx % spritesize) && !(inty % spritesize) && isCloseToInt;
}



AlphaSprite::AlphaSprite() {}

AlphaSprite::AlphaSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY, int width, int height,
	float posX, float posY) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY)
{
	SDL_SetTextureAlphaMod(this->_spriteSheet, 0);
}

AlphaSprite::~AlphaSprite() {}

void AlphaSprite::draw(Graphics& graphics, int x, int y) {
	if (!Effects::showTransitionEffect()) {
		return;
	}
	Sprite::draw(graphics, x, y);
}

void AlphaSprite::draw(Graphics& graphics, int x, int y, int width, int height) {
	if (!Effects::showTransitionEffect()) {
		return;
	}
	Sprite::draw(graphics, x, y, width, height);
}

void AlphaSprite::update(int alpha) {
	if (!Effects::showTransitionEffect()) {
		return;
	}
	this->_alpha = 255 - std::abs(alpha);
	SDL_SetTextureAlphaMod(this->_spriteSheet, this->_alpha);
}