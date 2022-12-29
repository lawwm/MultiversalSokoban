#include "player.h"
#include "graphics.h"
#include <iostream>

namespace player_constants {
	const float WALK_SPEED = 0.2f;
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

void Player::moveLeft() {
	this->_dx = -player_constants::WALK_SPEED;
	this->_dy = 0.0f;
	
	// set destination of left
	this->_destx = this->_x - 32;

	this->playAnimation("RunLeft");
	this->_facing = LEFT;
}

void Player::moveRight() {
	this->_dx = player_constants::WALK_SPEED;
	this->_dy = 0.0f;

	// set destination of right
	this->_destx = this->_x + 32;
	
	this->playAnimation("RunRight");
	this->_facing = RIGHT;
}

void Player::moveUp() {
	this->_dx = 0.0f;
	this->_dy = -player_constants::WALK_SPEED;

	// set destination of up
	this->_desty = this->_y - 32;

	this->playAnimation("RunUp");
	this->_facing = UP;
}

void Player::moveDown() {
	this->_dx = 0.0f;
	this->_dy = player_constants::WALK_SPEED;

	// set destination of down
	this->_desty = this->_y + 32;

	this->playAnimation("RunDown");
	this->_facing = DOWN;
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

//void handleTileCollisions
//Handles collisions with ALL tiles the player is colliding with
void Player::handleTileCollisions(std::vector<Rectangle>& others) {
	//Figure out what side the collision happened on and move the player accordingly
	for (int i = 0; i < others.size(); i++) {
		sides::Side collisionSide = Sprite::getCollisionSide(others.at(i));
		if (collisionSide != sides::NONE) {
			switch (collisionSide) {
			case sides::TOP:
				this->_y = others.at(i).getBottom() + 1;
				this->_dy = 0;
				break;
			case sides::BOTTOM:
				this->_y = others.at(i).getTop() - this->_boundingBox.getHeight() - 1;
				this->_dy = 0;
				break;
			case sides::LEFT:
				this->_x = others.at(i).getRight() + 1;
				break;
			case sides::RIGHT:
				this->_x = others.at(i).getLeft() - this->_boundingBox.getWidth() - 1;
				break;
			}

		}
	}
}

void Player::update(float elapsedTime, bool& isNotMoving) {
	
	if (_x == _destx && _y == _desty) {
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
