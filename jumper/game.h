#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "level.h"
#include "moveable.h"
#include <vector>

class Graphics;

class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	Player _player;
	Level _level;
	std::vector<Moveable> _moveables;
	bool _canPlayerMove = true;
};

#endif
