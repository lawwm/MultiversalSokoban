#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "zone.h"
#include "text.h"

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

	TextBox _textbox;
	Player _player;
	Zone _zone;
	Ticket _ticket = Ticket();
	bool _canPlayerMove = true;
	bool _canPlayerSwitchStage = true;
};

#endif
