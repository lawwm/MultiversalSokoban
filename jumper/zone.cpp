#include "zone.h"
#include "player.h"

Zone::Zone()
{
	
}

Zone::Zone(std::vector<zonedata> data, Graphics& graphics, int zonenumber)
{
	this->_data = data;
	this->_zonenumber = zonenumber;
	
	auto [paths, spawn, endpoint, moveablecoors] = data[zonenumber];
	this->_endpoint = EndPointSprite(graphics, endpoint);
	
	this->_stage = Stage(paths, spawn, graphics);
	for (Vector2 coors : moveablecoors) {
		this->_moveables.push_back(Moveable(graphics, coors));
	}
}

Zone::~Zone()
{
}

void Zone::nextZone(Graphics& graphics)
{
	this->_zonenumber = (this->_zonenumber + 1) % this->_data.size();
	
	this->selectZone(graphics, this->_zonenumber);
}

void Zone::selectZone(Graphics& graphics, int level)
{
	this->_zonenumber = level;
	auto [paths, spawn, endpoint, moveablecoors] = this->_data[this->_zonenumber];
	this->_endpoint = EndPointSprite(graphics, endpoint);

	this->_stage = Stage(paths, spawn, graphics);
	this->_moveables.clear();
	for (Vector2 coors : moveablecoors) {
		this->_moveables.push_back(Moveable(graphics, coors));
	}
}

Vector2 Zone::getSpawnPoint()
{
	return this->_stage.getPlayerSpawnPoint();
}

bool Zone::areAllMoveablesVisible()
{
	for (Moveable& movable : this->_moveables) {
		if (!movable.getVisible()) {
			return false;
		}
	}
	return true;
}

bool Zone::hasPlayerReachedEndPoint(Player& player)
{
	return player.getBoundingBox().collidesWith(this->_endpoint.getBoundingBox());
}

void Zone::restart(Graphics& graphics, int ticket)
{
	auto [paths, spawn, endpoint, moveablecoors] = this->_data[this->_zonenumber];
	for (int i = 0; i < moveablecoors.size(); ++i) {
		this->_moveables[i].restart(moveablecoors[i], ticket);
	}
	this->_stage.restart(ticket);
}


Stage& Zone::getStage()
{
	return this->_stage;
}

std::vector<Moveable>& Zone::getMoveables()
{
	return this->_moveables;
}

void Zone::draw(Graphics& graphics)
{
	this->_stage.draw(graphics);
	for (auto& m : this->_moveables) {
		m.draw(graphics);
	}
	this->_endpoint.draw(graphics);
}

void Zone::update(float elapsedTime, Graphics& graphics, bool& canPlayerSwitchStage)
{
	this->_stage.update(elapsedTime, canPlayerSwitchStage);
	for (auto& m : _moveables) {
		m.update(elapsedTime, this->_stage, graphics, canPlayerSwitchStage);
	}
	this->_endpoint.update(elapsedTime);
}

void Zone::undo(int ticket, bool& canPlayerSwitchStage)
{
	this->_stage.undo(ticket, canPlayerSwitchStage);
	for (auto& m : _moveables) {
		m.undo(ticket);
	}
}

