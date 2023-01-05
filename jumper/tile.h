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
	
	// copy constructor and operator
	Tile(const Tile& t);
	Tile& operator=(const Tile& t) noexcept;

	// move constructor and operator
	Tile(Tile&& t) noexcept;
	Tile& operator=(Tile&& t) noexcept;

	void update(int elapsedTime, const int& alpha);
	void draw(Graphics& graphics);
private:
	std::shared_ptr<SDL_Texture> _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
};


#endif
