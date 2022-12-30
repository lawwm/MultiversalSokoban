#ifndef PLAYER_H
#define PLAYER_H

#include "animatedsprite.h"
#include "globals.h"
#include "moveable.h"

class Graphics;

class Player : public AnimatedSprite {
public:
	Player();
	Player(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics);
	void update(float elapsedTime, bool& isNotMoving);

	/* void moveLeft
	 * Moves the player left by -dx
	 */
	void moveLeft(bool& isMoving, const std::vector<Rectangle>& levelCollisions);

	/* void moveRight
	 * Moves the player right by dx
	 */
	void moveRight(bool& isMoving, const std::vector<Rectangle>& levelCollisions);

	/* void moveUp
	 * Moves the player up by -dy
	 */
	void moveUp(bool& isMoving, const std::vector<Rectangle>& levelCollisions);

	/* void moveDown
	 * Moves the player down by dy
	 */
	void moveDown(bool& isMoving, const std::vector<Rectangle>& levelCollisions);

	/* void stopMoving
	 * Stops moving the player
	 */
	void stopMoving();

	/* bool canMoveToNewPosition
	*  check if it is possible to move to that position
	*/
	bool canMoveToNewPosition(const std::vector<Rectangle>& levelCollisions);

	virtual void animationDone(std::string currentAnimation);
	virtual void setupAnimations();

	const float getX() const;
	const float getY() const;

private:
	float _dx, _dy; // rate of change of travel

	float _destx, _desty; // final position for one by one travel

	Direction _facing;

	std::vector<Moveable> _pushing;
};

#endif

