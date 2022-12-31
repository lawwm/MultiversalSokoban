#include "moveable.h"
#include "graphics.h"
#include <iostream>
#include <tuple>
#include <utility>

Moveable::Moveable() {};

Moveable::Moveable(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, globals::coin, 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_facing(RIGHT)
{
	this->setupAnimations();
	this->playAnimation("Idle");
}


void Moveable::setupAnimations() {
	this->addAnimation(4, 0, 0, "Idle", 10, 10, Vector2(0, 0));
	this->addAnimation(1, 0, 0, "Moving", 10, 10, Vector2(0, 0));
}

void Moveable::animationDone(std::string currentAnimation) {}

const float Moveable::getX() const {
	return this->_x;
}

const float Moveable::getY() const {
	return this->_y;
}

void Moveable::undo(int ticket)
{
	if (_prevstates.empty() || std::get<0>(_prevstates.top()) != ticket) return;
	
	auto [ticketNumber, x, y, isVisible] = this->_prevstates.top();
	this->_prevstates.pop();
	this->_x = x;
	this->_y = y;
	this->setVisible(isVisible);
	this->playAnimation("Idle");
}

void Moveable::storeCurrState(int ticket)
{
	this->_prevstates.emplace(ticket, this->_x, this->_y, this->getVisible());
}

void Moveable::set(int x, int y) {
	this->_x = x;
	this->_y = y;
}

void Moveable::update(float elapsedTime, Stage& stage, Graphics& graphics) {
	Rectangle moveableBoxCurr(this->_x, this->_y, this->_sourceRect.w, this->_sourceRect.h);

	if (this->getVisible() && !stage.checkTileCollisions(moveableBoxCurr)) {
		this->setVisible(false);
		stage.addFx(new ExplosionSprite(graphics, Vector2(this->_x, this->_y)));
		return;
	}
	
	if ( ((int)this->_x) % 32 != 0 || ((int)this ->_y) % 32 != 0) {
		this->playAnimation("Moving");
	}
	else {
		this->playAnimation("Idle");
	}
	AnimatedSprite::update(elapsedTime);
}

void Moveable::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

bool Moveable::canMoveToNewPosition(const Stage& stage,
	std::vector<Moveable>& crates, std::vector<std::tuple<Moveable*, int, int>>& _pushing,
	std::pair<int, int> diff, int depth) {
	
	auto [x, y] = diff;
	Rectangle moveableBoxNext(this->_x + x, this->_y + y, this->_sourceRect.w, this->_sourceRect.h);
	_pushing.push_back({ this, x * depth, y * depth });

	// check for collision with other moveables
	for (int i = 0; i < crates.size(); ++i) {
		if (&crates[i] != this && crates[i].getVisible() && crates[i].getBoundingBox().collidesWith(moveableBoxNext)) {
			return crates[i].canMoveToNewPosition(stage, crates, _pushing, diff, depth+1);
		}
	}

	// check for collision with wall
	if (!stage.checkTileCollisions(moveableBoxNext)) return false;
	
	return true;
}