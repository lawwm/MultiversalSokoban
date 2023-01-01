#ifndef ZONE_H
#define ZONE_H

#include <vector>
#include <tuple>

#include "moveable.h"
#include "level.h"

class Zone {
public:
	Zone();
	Zone(std::vector<zonedata> data, Graphics& graphics, int zonenumber = 0);
	~Zone();
	Stage& getStage();
	std::vector<Moveable>& getMoveables();
	
	void draw(Graphics& graphics);
	void update(float elapsedTime, Graphics& graphics, bool& canPlayerSwitchStage);
	void undo(int ticket, bool& canPlayerSwitchStage);

	void nextZone(Graphics& graphics);
	Vector2 getSpawnPoint();

	bool hasWon(Player& player);
private:
	Stage _stage;
	std::vector<Moveable> _moveables;
	EndPointSprite _endpoint;
	int _zonenumber = 0;
	std::vector<zonedata> _data;
};

#endif
