#include "player.h"
#include "graphics.h"
#include <iostream>

namespace player_constants {
	const float WALK_SPEED = 0.15f;
}

Player::Player() {};

Player::Player(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, globals::eskimo, 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100),
	_dx(0),
	_dy(0),
	_facing(RIGHT),
	_destx(spawnPoint.x),
	_desty(spawnPoint.y)
{
	graphics.loadImage(globals::eskimo);

	this->setupAnimations();
	this->playAnimation("RunRight");
}


void Player::setupAnimations() {
	this->addAnimation(1, 0, 0, "IdleDown", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 16, 0, "IdleUp", 16, 16, Vector2(0, 0));		
	this->addAnimation(1, 32, 0, "IdleLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 48, 0, "IdleRight", 16, 16, Vector2(0, 0));

	
	this->addAnimation(4, 0, 0, "RunDown", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 16, 0, "RunUp", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 32, 0, "RunLeft", 16, 16, Vector2(0, 0));
	this->addAnimation(4, 48, 0, "RunRight", 16, 16, Vector2(0, 0));
}

void Player::animationDone(std::string currentAnimation) {}

const float Player::getX() const {
	return this->_x;
}

const float Player::getY() const {
	return this->_y;
}

bool Player::canMoveToNewPosition(const std::vector<Rectangle>& levelCollisions) {
	Rectangle playerBoxNext(this->_destx, this->_desty, this->_sourceRect.w, this->_sourceRect.h);
	for (int i = 0; i < levelCollisions.size(); i++) {
		if (levelCollisions.at(i).collidesWith(playerBoxNext)) {
			return false;
		}
	}
	return true;
}

void Player::moveLeft(bool& isMoving, const std::vector<Rectangle>& levelCollisions) {
	this->_dx = -player_constants::WALK_SPEED;
	this->_dy = 0.0f;
	
	// set destination of left
	this->_destx = this->_x - 32;

	this->playAnimation("RunLeft");
	this->_facing = LEFT;
	
	if (!canMoveToNewPosition(levelCollisions)) {
		this->_destx = this->_x;
		return;
	}	
	
	isMoving = false;
}

void Player::moveRight(bool& isMoving, const std::vector<Rectangle>& levelCollisions) {
	this->_dx = player_constants::WALK_SPEED;
	this->_dy = 0.0f;

	// set destination of right
	this->_destx = this->_x + 32;
	
	this->playAnimation("RunRight");
	this->_facing = RIGHT;
	
	if (!canMoveToNewPosition(levelCollisions)) {
		this->_destx = this->_x;
		return;
	}
	isMoving = false;
}

void Player::moveUp(bool& isMoving, const std::vector<Rectangle>& levelCollisions) {
	this->_dx = 0.0f;
	this->_dy = -player_constants::WALK_SPEED;

	// set destination of up
	this->_desty = this->_y - 32;

	this->playAnimation("RunUp");
	this->_facing = UP;

	if (!canMoveToNewPosition(levelCollisions)) {
		this->_desty = this->_y;
		return;
	}

	isMoving = false;
}

void Player::moveDown(bool& isMoving, const std::vector<Rectangle>& levelCollisions) {
	this->_dx = 0.0f;
	this->_dy = player_constants::WALK_SPEED;

	// set destination of down
	this->_desty = this->_y + 32;

	this->playAnimation("RunDown");
	this->_facing = DOWN;

	if (!canMoveToNewPosition(levelCollisions)) {
		this->_desty = this->_y;
		return;
	}

	isMoving = false;
}

void Player::stopMoving() {
	this->_dx = 0.0f;
	this->_dy = 0.0f;
	std::string idleDirection;
	switch (this->_facing) {
	case LEFT:
		idleDirection = "IdleLeft";
		break;
	case RIGHT:
		idleDirection = "IdleRight";
		break;
	case UP:
		idleDirection = "IdleUp";
		break;
	case DOWN:
		idleDirection = "IdleDown";
		break;
	}
	this->playAnimation(idleDirection);
}

void Player::update(float elapsedTime, bool& isNotMoving) {
	
	if (_x == _destx && _y == _desty) {
		//
		isNotMoving = true;
		return;
	}

	switch (this->_facing) {
	case LEFT:
		this->_x = std::max(this->_x + this->_dx * elapsedTime, _destx);
		break;
	case RIGHT:
		this->_x = std::min(this->_x + this->_dx * elapsedTime, _destx);
		break;
	case UP:
		this->_y = std::max(this->_y + this->_dy * elapsedTime, _desty);
		break;
	case DOWN:
		this->_y = std::min(this->_y + this->_dy * elapsedTime, _desty);
		break;
	}


	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}
