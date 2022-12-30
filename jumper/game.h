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
	Stage _stage;
	std::vector<Moveable> _moveables;
	bool _canPlayerMove = true;
	bool _canPlayerSwitchStage = true;
};

#endif
