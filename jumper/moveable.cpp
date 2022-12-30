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
	graphics.loadImage(globals::coin);

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

void Moveable::set(int x, int y) {
	this->_x = x;
	this->_y = y;
}

void Moveable::update(float elapsedTime) {
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

bool Moveable::canMoveToNewPosition(const std::vector<Rectangle>& levelCollisions,
	std::vector<Moveable>& crates, std::vector<std::tuple<Moveable*, int, int>>& _pushing,
	std::pair<int, int> diff, int depth) {
	
	auto [x, y] = diff;
	Rectangle moveableBoxNext(this->_x + x*depth, this->_y + y*depth, 
		this->_sourceRect.w, this->_sourceRect.h);
	_pushing.push_back({ this, x*depth, y*depth });

	for (int i = 0; i < crates.size(); ++i) {
		if (&crates[i] != this && crates[i].getBoundingBox().collidesWith(moveableBoxNext)
			&& !this->canMoveToNewPosition(levelCollisions, crates, _pushing, diff, depth+1)) {
			return false;
		}
	}

	for (int i = 0; i < levelCollisions.size(); i++) {
		if (levelCollisions.at(i).collidesWith(moveableBoxNext)) {
			return false;
		}
	}
	
	return true;
}