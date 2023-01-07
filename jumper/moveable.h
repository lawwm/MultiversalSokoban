#ifndef MOVEABLE_H
#define MOVEABLE_H

#include "level.h"
#include "animatedsprite.h"
#include "globals.h"
#include "audio.h"
#include "graphics.h"

#include <stack>
#include <iostream>
#include <tuple>
#include <utility>
#include <memory>

class Graphics;
class Stage;

class Moveable : public AnimatedSprite {
public:
	Moveable();
	Moveable(Graphics& graphics, Vector2 spawnPoint, std::string filePath, int sourceX, int sourceY, int width, int height, float timeToUpdate);

	/* bool canMoveToNewPosition
	*  check if it is possible to move to that position
	*/
	bool canMoveToNewPosition(const Stage& stage,
		std::vector<Moveable*>& crates, std::vector<std::tuple<Moveable*, int, int>>& _pushing,
		std::pair<int, int> diff, int depth);

	void set(int x, int y);

	const float getX() const;
	const float getY() const;

	virtual void animationDone(std::string currentAnimation)=0;
	virtual void setupAnimations()=0;
	
	virtual void undo(int ticket)=0;

	virtual void storeCurrState(int ticket)=0;

	virtual void restart(Vector2 spawn, int ticket)=0;
	
	virtual void draw(Graphics& graphics)=0;
	virtual void update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage)=0;
	virtual bool collidesWith(Rectangle other)=0;
	virtual bool isItPossibleToWin()=0;
	virtual bool hasItWon()=0;
};


class Coin : public Moveable { // to be pushed to endpoint
public:
	Coin();
	Coin(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics) override;
	void update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) override;

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	void undo(int ticket) override;

	void storeCurrState(int ticket) override;

	void restart(Vector2 spawn, int ticket) override;
	
	bool collidesWith(Rectangle other);
	bool isItPossibleToWin();
	bool hasItWon();
private:
	LimitedStack<std::tuple<int, int, int, int>> _prevstates; // ticket number, x, y, visible,
};
	
class Sushi : public Moveable { // to be destroyed in order to win
public:
	Sushi();
	Sushi(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics) override;
	void update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) override;

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	void undo(int ticket) override;

	void storeCurrState(int ticket) override;

	void restart(Vector2 spawn, int ticket) override;
	
	bool collidesWith(Rectangle other);
	bool isItPossibleToWin();
	bool hasItWon();
private:
	LimitedStack<std::tuple<int, int, int, int>> _prevstates; // ticket number, x, y, visible,
};

class Bomb : public Moveable { // it destroys the tile
public:
	Bomb();
	Bomb(Graphics& graphics, Vector2 spawnPoint);
	void draw(Graphics& graphics) override;
	void update(float elapsedTime, Stage& stage, Graphics& graphics, bool& canPlayerSwitchStage) override;

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	void undo(int ticket) override;

	void storeCurrState(int ticket) override;

	void restart(Vector2 spawn, int ticket) override;

	bool collidesWith(Rectangle other);
	bool isItPossibleToWin();
	bool hasItWon();
private:
	LimitedStack<std::tuple<int, int, int, int>> _prevstates; // ticket number, x, y, visible,
};

#endif

