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
	void update(float elapsedTime, const std::vector<Rectangle>& levelCollisions);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	/* bool canMoveToNewPosition
	*  check if it is possible to move to that position
	*/
	bool canMoveToNewPosition(const std::vector<Rectangle>& levelCollisions,
		std::vector<Moveable>& crates, std::vector<std::tuple<Moveable*, int, int>>& _pushing,
		std::pair<int, int> diff, int depth);

	void set(int x, int y);

	const float getX() const;
	const float getY() const;

private:
	Direction _facing;
};

#endif

