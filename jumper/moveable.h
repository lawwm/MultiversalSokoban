#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "animatedsprite.h"
#include "globals.h"

class Graphics;

class Moveable : public AnimatedSprite {
public:
	Moveable();
	Moveable(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	const float getX() const;
	const float getY() const;

private:
	Direction _facing;
};

#endif

