#ifndef GAME_H
#define GAME_H

#include "player.h"
#include "zone.h"
#include "text.h"
#include "globals.h"

#include <vector>

class Graphics;
class Input;
class Game {
public:
	Game();
	~Game();
private:
	void gameLoop();
	void draw(Graphics& graphics);
	void update(float elapsedTime, Graphics& graphics);
	void undo();
	void restart(Graphics& graphics);

	bool individualZone(Graphics& graphics, Input& input, int& LAST_UPDATE_TIME);
	bool overworld(Graphics& graphics, Input& input, int& LAST_UPDATE_TIME);
	
	void initialisePlayer(Graphics& graphics);

	Stage _overworld;
	TextBox _textbox;
	Player _player;
	Zone _zone;
	Ticket _ticket = Ticket();
	
	Screen _currScreen =  OVERWORLD;	
	
	bool _canPlayerMove = true;
	bool _canPlayerSwitchStage = true;
	int _isPlayerInLevelSelect = 0;

};

#endif
