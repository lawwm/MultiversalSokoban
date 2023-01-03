#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "animatedsprite.h"
#include "level.h"
#include "globals.h"
#include "audio.h"

#include <stack>

class Graphics;

class Moveable : public AnimatedSprite {
public:
	Moveable();
	Moveable(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics) override;
	void update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage);

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

	void undo(int ticket);

	void storeCurrState(int ticket);

	void restart(Vector2 spawn, int ticket);
	
private:
	Direction _facing;
	std::stack<std::tuple<int, int, int, int>> _prevstates; // ticket number, x, y, visible,
	Foley _foley;
};

#endif

