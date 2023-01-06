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

bool Player::canMoveToNewPosition(const Stage& stage, std::vector<Moveable*>& crates,
	const std::pair<int, int>& diff) {
	const Rectangle playerBoxNext(this->_destx, this->_desty, this->_sourceRect.w, this->_sourceRect.h);
	
	
	if (!stage.checkTileCollisions(playerBoxNext) || !stage.checkTilePoison(playerBoxNext)) return false;
	for (int i = 0; i < crates.size(); ++i) {
		if (crates[i]->getVisible() && crates[i]->getBoundingBox().collidesWith(playerBoxNext)) {
			return crates[i]->canMoveToNewPosition(stage, crates, this->_pushing, diff, 1);
		}
	}

	
	return true;
}

void Player::moveLeft(bool& isMoving, const Stage& stage,
	 std::vector<Moveable*>& crates, Ticket& ticket) {
	move(isMoving, stage, crates, -player_constants::WALK_SPEED, 0.0f,
		-32, 0, "RunLeft", LEFT, ticket);
}

void Player::moveRight(bool& isMoving, const Stage& stage,
	 std::vector<Moveable*>& crates, Ticket& ticket) {
	move(isMoving, stage, crates, player_constants::WALK_SPEED, 0.0f,
		32, 0, "RunRight", RIGHT, ticket);
}

void Player::moveUp(bool& isMoving, const Stage& stage,
	 std::vector<Moveable*>& crates, Ticket& ticket) {
	move(isMoving, stage, crates, 0.0f, -player_constants::WALK_SPEED,
		0, -32, "RunUp", UP, ticket);
}

void Player::moveDown(bool& isMoving, const Stage& stage,
	 std::vector<Moveable*>& crates, Ticket& ticket) {
	move(isMoving, stage, crates, 0.0f, player_constants::WALK_SPEED,
		0, 32, "RunDown", DOWN, ticket);
}

void Player::move(bool& isMoving, const Stage& stage,
	std::vector<Moveable*>& crates, const float setdx, const float setdy,
	int xdiff, int ydiff, std::string animation, Direction direction, Ticket& ticket) {

	// set the speed of travel
	this->_dx = setdx;
	this->_dy = setdy;

	// set destination of next tile
	this->_destx = this->_x + xdiff;
	this->_desty = this->_y + ydiff;
	
	// set animation and direction
	this->playAnimation(animation);
	this->_facing = direction;

	// check if able to move to new position
	if (!canMoveToNewPosition(stage, crates, { xdiff, ydiff })) {
		Foley::playSound("collide");
		this->_destx = this->_x;
		this->_desty = this->_y;
		return;
	}
	
	Foley::playSound("walk");

	// store previous state within stack
	int generatedTicket = ticket.insertTicket();
	this->storeCurrState(generatedTicket);
	for (auto& crate : _pushing) {
		std::get<0>(crate)->storeCurrState(generatedTicket);
	}

	isMoving = false;
}

void Player::storeCurrState(int ticket) {
	_prevstates.emplace(ticket, this->_x, this->_y, this->getVisible(), this->_facing);
}

bool Player::isStationary() {
	int intx = floor(this->_x);
	int inty = floor(this->_y);
	bool isCloseToInt = std::abs(this->_x - floor(this->_x)) < 0.000000001f && std::abs(this->_y - floor(this->_y)) < 0.000000001f;
	int spritesize = floor(globals::SPRITE_SCALE * globals::SPRITE_WIDTH);
	return !(intx % spritesize) && !(inty % spritesize) && isCloseToInt;
}

void Player::restart(Vector2 spawn, int ticket)
{
	this->storeCurrState(ticket);
	this->_x = spawn.x;
	this->_destx = spawn.x;
	this->_y = spawn.y;
	this->_desty = spawn.y;
	this->_dx = 0.0f;
	this->_dy = 0.0f;
	this->_facing = RIGHT;
	this->setVisible(true);
	this->stopMoving();
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

void Player::update(float elapsedTime, bool& isNotMoving, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) {
	Rectangle playerCurr(this->_x, this->_y, this->_sourceRect.w, this->_sourceRect.h);
	
	// if player collides with a hitbox, he dies, display animation
	if (this->getVisible() && (!stage.checkTileCollisions(playerCurr) || !stage.checkTilePoison(playerCurr)) && canPlayerSwitchStage) {
		Foley::playSound("kill");
		this->setVisible(false);
		stage.addFx(std::make_unique<ExplosionSprite>(graphics, Vector2(this->_x, this->_y)));
		return;
	}

	// if player has reached destination, stop moving and pushing
	if (_x == _destx && _y == _desty) {
		this->_pushing.clear();
		isNotMoving = true;
		return;
	}

	// calculate position depending on direction faced
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

	// observer pattern, change the position of items that player is pushing
	for (int i = 0; i < this->_pushing.size(); ++i) {
		auto [mptr, xdist, ydist] = _pushing[i];
		
		mptr->set(this->_x + xdist, this->_y + ydist);
	}

	AnimatedSprite::update(elapsedTime);
}

void Player::draw(Graphics& graphics) {
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void Player::undo(int ticket)
{
	if (_prevstates.empty() || std::get<0>(_prevstates.top()) != ticket) return;
	
	auto [ticketNumber, x, y, isVisible, dir] = this->_prevstates.top();
	this->_prevstates.pop();
	this->_x = x;
	this->_destx = x;
	this->_y = y;
	this->_desty = y;
	this->_dx = 0.0f;
	this->_dy = 0.0f;
	this->setVisible(isVisible);
	this->_facing = dir;
	this->stopMoving();
}