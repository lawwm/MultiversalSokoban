#include "tile.h"
#include "graphics.h"

#include "SDL.h"
#include <iostream>
Tile::Tile() {}

Tile::Tile(std::shared_ptr<SDL_Texture> tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position) :
	_tileset(tileset),
	_size(size),
	_tilesetPosition(tilesetPosition),
	_position(Vector2(position.x* globals::SPRITE_SCALE, position.y* globals::SPRITE_SCALE))
{
	//std::cout << "create" << std::endl;
}

Tile::Tile(const Tile& t) 
	: _tileset(t._tileset), _size(t._size), _tilesetPosition(t._tilesetPosition), 
	_position(t._position)
{
	//std::cout << "tile copy constructor" << std::endl;
}

Tile& Tile::operator=(const Tile& t) noexcept
{
	if (this == &t) return *this;

	//std::cout << "tile copy opeartor" << std::endl;

	this->_size = t._size;
	this->_tilesetPosition = t._tilesetPosition;
	this->_position = t._position;
	this->_tileset = t._tileset;

	return *this;
}

Tile::Tile(Tile&& t) noexcept 
	: _tileset(std::exchange(t._tileset, nullptr)), _size(t._size), 
	_tilesetPosition(t._tilesetPosition), _position(t._position)
{
	//std::cout << "tile move constructor" << std::endl;
}

Tile& Tile::operator=(Tile&& t) noexcept
{
	if (this == &t) return *this;

	//std::cout << "tile move operator" << std::endl;

	this->_tileset = std::exchange(t._tileset, nullptr);
	this->_size = t._size;
	this->_tilesetPosition = t._tilesetPosition;
	this->_position = t._position;

	return *this;
}

void Tile::update(int elapsedTime, const int& alpha) {
	SDL_SetTextureColorMod(_tileset.get(), alpha, alpha, alpha);
}

void Tile::draw(Graphics& graphics) {
	SDL_Rect destRect = { this->_position.x, this->_position.y,
			this->_size.x * globals::SPRITE_SCALE, this->_size.y * globals::SPRITE_SCALE };
	SDL_Rect sourceRect = { this->_tilesetPosition.x, this->_tilesetPosition.y, this->_size.x, this->_size.y };

	graphics.blitSurface(this->_tileset.get(), &sourceRect, &destRect);
}
