#include "tile.h"


Tile::Tile() {}

Tile::Tile(std::shared_ptr<SDL_Texture> tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position, bool isDestructible) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x* Window::getSpriteScale(), position.y* Window::getSpriteScale())),
	_isDestructible(isDestructible)
{
}

Tile::Tile(const Tile& t) 
	: _tileset(t._tileset), _size(t._size), _tilesetPosition(t._tilesetPosition), 
	_position(t._position), _isDestructible(t._isDestructible)
{
}

Tile& Tile::operator=(const Tile& t) noexcept
{
	if (this == &t) return *this;

	this->_size = t._size;
	this->_tilesetPosition = t._tilesetPosition;
	this->_position = t._position;
	this->_tileset = t._tileset;
	this->_isDestructible = t._isDestructible;

	return *this;
}

Tile::Tile(Tile&& t) noexcept 
	: _tileset(std::exchange(t._tileset, nullptr)), _size(t._size), 
	_tilesetPosition(t._tilesetPosition), _position(t._position), _isDestructible(t._isDestructible)
{
}

Tile& Tile::operator=(Tile&& t) noexcept
{
	if (this == &t) return *this;

	this->_tileset = std::exchange(t._tileset, nullptr);
	this->_size = t._size;
	this->_tilesetPosition = t._tilesetPosition;
	this->_position = t._position;
	this->_isDestructible = t._isDestructible;

	return *this;
}

void Tile::update(int elapsedTime, const int& alpha) {
	SDL_SetTextureColorMod(_tileset.get(), alpha, alpha, alpha);
}

void Tile::draw(Graphics& graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y,
			this->_size.x * Window::getSpriteScale(), this->_size.y * Window::getSpriteScale() };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

	graphics.blitSurface(this->_tileset.get(), &sourceRect, &destRect);
}


Vector2 Tile::getPosition() const { 
	return this->_position; 
}

bool Tile::isDestructible() const {
	return this->_isDestructible;
}

int Tile::getScreenKeyValue(int x, int y)
{
	return x + Window::getScreenWidth() * y;
}

