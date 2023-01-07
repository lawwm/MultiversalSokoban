#include "zone.h"


Zone::Zone()
{
	
}

Zone::Zone(std::vector<zonedata> data, Graphics& graphics, int zonenumber)
{
	this->_data = data;
	this->_zonenumber = zonenumber;
	auto [paths] = data[zonenumber];
	
	this->_stage = std::move(Stage(paths, graphics));
}

Zone::~Zone()
{
}

Zone::Zone(Zone&& t) noexcept
{
	//std::cout << "tile move constructor" << std::endl;
	this->_stage = std::move(t._stage);
	this->_zonenumber = t._zonenumber;
	this->_data = std::move(t._data);

	t._data.clear();
}

Zone& Zone::operator=(Zone&& t) noexcept
{
	if (this == &t) return *this;

	this->_stage = std::move(t._stage);
	this->_zonenumber = t._zonenumber;
	this->_data = std::move(t._data);

	t._data.clear();

	return *this;
}

bool Zone::nextZone(Graphics& graphics, Screen& currScreen)
{
	if (this->_zonenumber + 1 == this->_data.size()) {
		currScreen = VICTORY_SCREEN;
		return true;
	}
	
	this->_zonenumber += 1;
	this->selectZone(graphics, this->_zonenumber);

	return false;
}

void Zone::selectZone(Graphics& graphics, int level)
{
	this->_zonenumber = level;
	auto [paths] = this->_data[this->_zonenumber];
	
	this->_stage = std::move(Stage(paths, graphics));
}

Vector2 Zone::getSpawnPoint()
{
	return this->_stage.getPlayerSpawnPoint();
}

bool Zone::hasPlayerWon(Player& player)
{
	return this->_stage.hasPlayerWon(player);
}

bool Zone::isItPossibleToWin()
{
	return this->_stage.isItPossibleToWin();
}

void Zone::restart(Graphics& graphics, int ticket)
{
	this->_stage.restart(ticket);
}


Stage& Zone::getStage()
{
	return this->_stage;
}

std::vector<Moveable*>& Zone::getMoveables()
{
	return this->_stage.getMoveables();
}

int Zone::getZoneNumber()
{
	return this->_zonenumber;
}

void Zone::draw(Graphics& graphics)
{
	this->_stage.draw(graphics);
}

void Zone::update(float elapsedTime, Graphics& graphics, bool& canPlayerSwitchStage)
{
	this->_stage.update(elapsedTime, canPlayerSwitchStage, graphics);
}

void Zone::undo(int ticket, bool& canPlayerSwitchStage)
{
	this->_stage.undo(ticket, canPlayerSwitchStage);
}

