#ifndef TILE_H
#define TILE_H

#include "globals.h";
#include "SDL.h";
#include <iostream>
#include <memory>
#include "graphics.h"

struct SDL_Texture;
class Graphics;

class Tile {
public:
	Tile();
	Tile(std::shared_ptr<SDL_Texture> tileset, Vector2 size, Vector2 tilesetPosition, Vector2 position, bool isDestructible=false);
	~Tile() {
	}
	
	// copy constructor and operator
	Tile(const Tile& t);
	Tile& operator=(const Tile& t) noexcept;

	// move constructor and operator
	Tile(Tile&& t) noexcept;
	Tile& operator=(Tile&& t) noexcept;

	void update(int elapsedTime, const int& alpha);
	void draw(Graphics& graphics);

	Vector2 getPosition() const;
	bool isDestructible() const;

	static int getScreenKeyValue(int x, int y);
	
private:
	std::shared_ptr<SDL_Texture> _tileset;
	Vector2 _size;
	Vector2 _tilesetPosition;
	Vector2 _position;
	bool _isDestructible;
};


#endif
