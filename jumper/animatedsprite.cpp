#include "animatedsprite.h"


/* AnimatedSprite class
 * Animates our sprites
 */

AnimatedSprite::AnimatedSprite() {}

AnimatedSprite::AnimatedSprite(Graphics& graphics, const std::string& filePath, int sourceX, int sourceY,
	int width, int height, float posX, float posY, float timeToUpdate) :
	Sprite(graphics, filePath, sourceX, sourceY, width, height, posX, posY),
	_frameIndex(0),
	_timeToUpdate(timeToUpdate),
	_visible(true),
	_currentAnimationOnce(true),
	_currentAnimation(""),
	_timeElapsed(0)
{}

void AnimatedSprite::addAnimation(int frames, int x, int y, std::string name, int width, int height, Vector2 offset) {
	std::vector<SDL_Rect> rectangles;
	for (int i = 0; i < frames; i++) {
		SDL_Rect newRect = { x, (y + i) * height, width, height };
		rectangles.push_back(newRect);
	}
	this->_animations.insert(std::pair<std::string, std::vector<SDL_Rect> >(name, rectangles));
	this->_offsets.insert(std::pair<std::string, Vector2>(name, offset));
}

void AnimatedSprite::resetAnimations() {
	this->_animations.clear();
	this->_offsets.clear();
}

void AnimatedSprite::playAnimation(std::string animation, bool once) {
	this->_currentAnimationOnce = once;
	if (this->_currentAnimation != animation) {
		this->_currentAnimation = animation;
		this->_frameIndex = 0;
	}
}

void AnimatedSprite::setVisible(bool visible) {
	this->_visible = visible;
}

bool AnimatedSprite::getVisible()
{
	return this->_visible;
}

void AnimatedSprite::stopAnimation() {
	this->_frameIndex = 0;
	this->animationDone(this->_currentAnimation);
}

void AnimatedSprite::update(int elapsedTime) {
	Sprite::update();
	
	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		if (this->_frameIndex < this->_animations[this->_currentAnimation].size() - 1) {
			this->_frameIndex++;
		}
		else {
			if (this->_currentAnimationOnce == true) {
				this->setVisible(false);
			}
			this->_frameIndex = 0;
			this->animationDone(this->_currentAnimation);
		}
	}
}

void AnimatedSprite::draw(Graphics& graphics, int x, int y) {
	if (this->_visible) {
		SDL_Rect destinationRectangle;
		destinationRectangle.x = x + this->_offsets[this->_currentAnimation].x * Window::getSpriteScale();
		destinationRectangle.y = y + this->_offsets[this->_currentAnimation].y * Window::getSpriteScale();
		destinationRectangle.w = this->_sourceRect.w * Window::getSpriteScale();
		destinationRectangle.h = this->_sourceRect.h * Window::getSpriteScale();

		SDL_Rect sourceRect = this->_animations[this->_currentAnimation][this->_frameIndex];
		graphics.blitSurface(this->_spriteSheet, &sourceRect, &destinationRectangle);
	}
}

ExplosionSprite::ExplosionSprite()
{
}

ExplosionSprite::ExplosionSprite(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, globals::explode, 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100)
{
	this->setupAnimations();
	this->playAnimation("Explode", true);
}

void ExplosionSprite::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void ExplosionSprite::update(float elapsedTime)
{
	AnimatedSprite::update(elapsedTime);
}

void ExplosionSprite::animationDone(std::string currentAnimation)
{
}

void ExplosionSprite::setupAnimations()
{
	this->addAnimation(6, 0, 0, "Explode", 32, 32, Vector2(0, 0));
}




EndPointSprite::EndPointSprite()
{
}

EndPointSprite::EndPointSprite(Graphics& graphics, Vector2 spawnPoint) :
	AnimatedSprite(graphics, globals::endpoint, 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100)
{
	this->setupAnimations();
	this->playAnimation("Glow");
}

EndPointSprite::EndPointSprite(Graphics& graphics, Vector2 spawnPoint, bool visibility) :
	AnimatedSprite(graphics, globals::endpoint, 0, 0, 16, 16, spawnPoint.x, spawnPoint.y, 100)
{
	this->setupAnimations();
	this->playAnimation("Glow");
	this->setVisible(visibility);
}

void EndPointSprite::draw(Graphics& graphics)
{
	AnimatedSprite::draw(graphics, this->_x, this->_y);
}

void EndPointSprite::update(float elapsedTime)
{
	AnimatedSprite::update(elapsedTime);
}

void EndPointSprite::animationDone(std::string currentAnimation)
{
}

void EndPointSprite::setupAnimations()
{
	this->addAnimation(6, 0, 0, "Glow", 47, 47, Vector2(0, 0));
}
