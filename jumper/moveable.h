#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "animatedsprite.h"
#include "level.h"
#include "globals.h"

class Graphics;

class Moveable : public AnimatedSprite {
public:
	Moveable();
	Moveable(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics) override;
	void update(float elapsedTime, Stage& stage, Graphics& graphics);

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	/* bool canMoveToNewPosition
	*  check if it is possible to move to that position
	*/
	bool canMoveToNewPosition(const Stage& stage,
		std::vector<Moveable>& crates, std::vector<std::tuple<Moveable*, int, int>>& _pushing,
		std::pair<int, int> diff, int depth);

	void set(int x, int y);

	const float getX() const;
	const float getY() const;

private:
	Direction _facing;
};

#endif

