#include "tiletype.h"

CompletionSprite::CompletionSprite(Graphics& graphics, Vector2 spawnPoint, int levelNumber, bool isCompleted) :
	AnimatedSprite(graphics, globals::chest, 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100)
{
	this->setupAnimations();
	this->_levelNumber = levelNumber;
	this->_isCompleted = isCompleted;
	this->playAnimation("Close");
}

void CompletionSprite::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void CompletionSprite::update(float elapsedTime)
{
	if (this->_isCompleted == true) {
		this->playAnimation("Open");
	}
	AnimatedSprite::update(elapsedTime);
}

void CompletionSprite::animationDone(std::string currentAnimation)
{
}

void CompletionSprite::setupAnimations()
{
	this->addAnimation(1, 0, 0, "Close", 16, 16, Vector2(0, 0));
	this->addAnimation(1, 16, 0, "Open", 16, 16, Vector2(0, 0));
}

int CompletionSprite::getLevelNumber() const
{
	return this->_levelNumber;
}

bool CompletionSprite::getCompleted() const
{
	return this->_isCompleted;
}

void CompletionSprite::setCompleted(bool isCompleted)
{
	this->_isCompleted = isCompleted;
}