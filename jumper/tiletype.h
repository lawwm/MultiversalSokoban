#ifndef TILETYPE_H
#define TILETYPE_H

#include "animatedsprite.h"
class CompletionSprite : public AnimatedSprite
{
public:
	CompletionSprite();
	CompletionSprite(Graphics& graphics, Vector2 spawnPoint, int levelNumber, bool isCompleted);
	void draw(Graphics& graphics);
	void update(float elapsedTime);

	void animationDone(std::string currentAnimation) override;
	void setupAnimations() override;

	int getLevelNumber() const;
	bool getCompleted() const;
	void setCompleted(bool isCompleted);

private:
	int _levelNumber;
	bool _isCompleted;
};

#endif