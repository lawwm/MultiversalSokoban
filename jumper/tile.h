#ifndef TILE_H
#define TILE_H

#include "globals.h";
#include "SDL.h";
#include <iostream>
#include <memory>

struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(std::shared_ptr<SDL_Texture> tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position);

	~Tile() {
		//std::cout << "destroy" << std::endl;
		//SDL_DestroyTexture(this->_tileset);
	}
	
	Tile(const Tile& t) : _tileset(t._tileset), _size(t._size), _tilesetPosition(t._tilesetPosition), _position(t._position)
	{
		//std::cout << "tile copy constructor" << std::endl;
	}
	 
	Tile& operator=(const Tile& t) noexcept
	{
		if (this == &t) return *this;
		
		//std::cout << "tile copy opeartor" << std::endl;

		this->_size = t._size;
		this->_tilesetPosition = t._tilesetPosition;
		this->_position = t._position;
		this->_tileset = t._tileset;
		
		return *this;
	}


	Tile(Tile&& t) noexcept : _tileset(std::exchange(t._tileset, nullptr)), _size(t._size), _tilesetPosition(t._tilesetPosition), _position(t._position) 
	{
		//std::cout << "tile move constructor" << std::endl;
	}

	Tile& operator=(Tile&& t) noexcept
	{
		if (this == &t) return *this;

		//std::cout << "tile move operator" << std::endl;

		this->_tileset = std::exchange(t._tileset, nullptr);
		this->_size = t._size;
		this->_tilesetPosition = t._tilesetPosition;
		this->_position = t._position;

		return *this;
	}

	void update(int elapsedTime, const int& alpha);
	void draw(Graphics& graphics);
private:
	std::shared_ptr<SDL_Texture> _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
};


#endif
