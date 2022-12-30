#include "moveable.h"
#include "graphics.h"
#include <iostream>

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

void Moveable::update(float elapsedTime) {
	AnimatedSprite::update(elapsedTime);
}

void Moveable::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}
