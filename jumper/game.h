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
	void update(float elapsedTime, Graphics& graphics);
	void undo();

	
	Player _player;
	Stage _stage;
	std::vector<Moveable> _moveables;
	Ticket _ticket = Ticket();
	bool _canPlayerMove = true;
	bool _canPlayerSwitchStage = true;
};

#endif
