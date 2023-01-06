#include "moveable.h"
#include "graphics.h"

#include <iostream>
#include <tuple>
#include <utility>
#include <memory>

/**
Base class for pushing functionality
*/

Moveable::Moveable() {};

Moveable::Moveable(Graphics& graphics, Vector2 spawnPoint, std::string filePath, int sourceX, int sourceY, int width, int height, float timeToUpdate) :
	AnimatedSprite(graphics, filePath, sourceX, sourceY, width, height, spawnPoint.x, spawnPoint.y, timeToUpdate)
{
}

const float Moveable::getX() const {
	return this->_x;
}

const float Moveable::getY() const {
	return this->_y;
}

void Moveable::set(int x, int y) {
	this->_x = x;
	this->_y = y;
}

bool Moveable::canMoveToNewPosition(const Stage& stage,
	std::vector<Moveable*>& crates, std::vector<std::tuple<Moveable*, int, int>>& _pushing,
	std::pair<int, int> diff, int depth) {
	
	auto [x, y] = diff;
	Rectangle moveableBoxNext(this->_x + x, this->_y + y, this->_sourceRect.w, this->_sourceRect.h);
	_pushing.push_back({ this, x * depth, y * depth });

	// check for collision with other moveables
	for (int i = 0; i < crates.size(); ++i) {
		if (crates[i] != this && crates[i]->getVisible() && crates[i]->getBoundingBox().collidesWith(moveableBoxNext)) {
			return crates[i]->canMoveToNewPosition(stage, crates, _pushing, diff, depth+1);
		}
	}

	// check for collision with wall
	if (!stage.checkTileCollisions(moveableBoxNext)) return false;
	
	return true;
}

/**
Coin class
It is a moveable which is meant to be at endpoint in order for player to win
*/

Coin::Coin() {};

Coin::Coin(Graphics& graphics, Vector2 spawnPoint) :
	Moveable(graphics, spawnPoint, globals::coin, 0, 0, 12, 12, 100)
{
	this->setupAnimations();
	this->playAnimation("Idle");
}

void Coin::setupAnimations() {
	this->addAnimation(4, 0, 0, "Idle", 10, 10, Vector2(2, 2));
	this->addAnimation(1, 0, 0, "Moving", 10, 10, Vector2(2, 2));
}

void Coin::animationDone(std::string currentAnimation) {}


void Coin::undo(int ticket)
{
	if (_prevstates.empty() || std::get<0>(_prevstates.top()) != ticket) return;
	
	auto [ticketNumber, x, y, isVisible] = this->_prevstates.top();
	this->_prevstates.pop();
	this->_x = x;
	this->_y = y;
	this->setVisible(isVisible);
	this->playAnimation("Idle");
}

void Coin::storeCurrState(int ticket)
{
	this->_prevstates.emplace(ticket, this->_x, this->_y, this->getVisible());
}

void Coin::restart(Vector2 spawn, int ticket)
{
	this->storeCurrState(ticket);
	this->_x = spawn.x;
	this->_y = spawn.y;
	this->setVisible(true);
}

void Coin::update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) {
	Rectangle moveableBoxCurr(this->_x, this->_y, this->_sourceRect.w, this->_sourceRect.h);

	// if a moveable collides with a hit box, it dies.
	if (this->getVisible() && !stage.checkTileCollisions(moveableBoxCurr) && canPlayerSwitchStage) {
		this->setVisible(false);
		Foley::playSound("kill");
		stage.addFx(std::make_unique<ExplosionSprite>(graphics, Vector2(this->_x, this->_y)));
		return;
	}

	if (((int)this->_x) % 32 != 0 || ((int)this->_y) % 32 != 0) {
		this->playAnimation("Moving");
	}
	else {
		this->playAnimation("Idle");
	}
	AnimatedSprite::update(elapsedTime);
}

void Coin::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

bool Coin::collidesWith(Rectangle other) {
	return this->getBoundingBox().collidesWith(other);
};

bool Coin::isItPossibleToWin() {
	return this->getVisible();
};

bool Coin::hasItWon() {
	return this->getVisible();
};

/**
Sushi class
It is a moveable which is meant to be destroyed in order for player to win
*/

Sushi::Sushi() {};

Sushi::Sushi(Graphics& graphics, Vector2 spawnPoint) :
	Moveable(graphics, spawnPoint, globals::sushi, 0, 0, 16, 16, 100)
{
	this->setupAnimations();
	this->playAnimation("Idle");
}

void Sushi::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 16, 16, Vector2(0, 0));
}

void Sushi::animationDone(std::string currentAnimation) {}


void Sushi::undo(int ticket)
{
	if (_prevstates.empty() || std::get<0>(_prevstates.top()) != ticket) return;

	auto [ticketNumber, x, y, isVisible] = this->_prevstates.top();
	this->_prevstates.pop();
	this->_x = x;
	this->_y = y;
	this->setVisible(isVisible);
	this->playAnimation("Idle");
}

void Sushi::storeCurrState(int ticket)
{
	this->_prevstates.emplace(ticket, this->_x, this->_y, this->getVisible());
}

void Sushi::restart(Vector2 spawn, int ticket)
{
	this->storeCurrState(ticket);
	this->_x = spawn.x;
	this->_y = spawn.y;
	this->setVisible(true);
}

void Sushi::update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) {
	Rectangle moveableBoxCurr(this->_x, this->_y, this->_sourceRect.w, this->_sourceRect.h);

	// if a moveable collides with a hit box, it dies.
	if (this->getVisible() && !stage.checkTileCollisions(moveableBoxCurr) && canPlayerSwitchStage) {
		this->setVisible(false);
		Foley::playSound("kill");
		stage.addFx(std::make_unique<ExplosionSprite>(graphics, Vector2(this->_x, this->_y)));
		return;
	}

	AnimatedSprite::update(elapsedTime);
}

void Sushi::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

bool Sushi::collidesWith(Rectangle other) {
	return false;
};

bool Sushi::isItPossibleToWin() {
	return true;
};

bool Sushi::hasItWon() {
	return !this->getVisible();
};

/**
Bomb class
It is a moveable which can destroy tiles
*/

Bomb::Bomb() {};

Bomb::Bomb(Graphics& graphics, Vector2 spawnPoint) :
	Moveable(graphics, spawnPoint, globals::bomb, 0, 0, 10, 10, 100)
{
	this->setupAnimations();
	this->playAnimation("Idle");
}

void Bomb::setupAnimations() {
	this->addAnimation(1, 0, 0, "Idle", 9, 9, Vector2(3, 3));
}

void Bomb::animationDone(std::string currentAnimation) {}


void Bomb::undo(int ticket)
{
	if (_prevstates.empty() || std::get<0>(_prevstates.top()) != ticket) return;

	auto [ticketNumber, x, y, isVisible] = this->_prevstates.top();
	this->_prevstates.pop();
	this->_x = x;
	this->_y = y;
	this->setVisible(isVisible);
	this->playAnimation("Idle");
}

void Bomb::storeCurrState(int ticket)
{
	this->_prevstates.emplace(ticket, this->_x, this->_y, this->getVisible());
}

void Bomb::restart(Vector2 spawn, int ticket)
{
	this->storeCurrState(ticket);
	this->_x = spawn.x;
	this->_y = spawn.y;
	this->setVisible(true);
}

void Bomb::update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) {
	Rectangle moveableBoxCurr(this->_x, this->_y, this->_sourceRect.w, this->_sourceRect.h);

	// if a moveable collides with a hit box, it dies.
	if (this->getVisible() && !stage.checkTileCollisions(moveableBoxCurr) && canPlayerSwitchStage) {
		this->setVisible(false);
		Foley::playSound("kill");
		stage.addFx(std::make_unique<ExplosionSprite>(graphics, Vector2(this->_x, this->_y)));
		return;
	}

	AnimatedSprite::update(elapsedTime);
}

void Bomb::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

bool Bomb::collidesWith(Rectangle other) {
	return false;
};

bool Bomb::isItPossibleToWin() {
	return true;
};

bool Bomb::hasItWon() {
	return !this->getVisible();
};