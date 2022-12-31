#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <stack>

#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "animatedsprite.h"

class Graphics;
class Moveable;
class Player;
struct SDL_Texture;

struct Tileset {
	SDL_Texture* Texture;
	int FirstGid;
	Tileset() {
		this->FirstGid = -1;
	}
	Tileset(SDL_Texture* texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};

class Level {
public:
	Level();
	Level(std::string mapName, Vector2 spawnPoint, Graphics& graphics);
	~Level();
	void update(int elapsedTime, const int& alpha);
	void draw(Graphics& graphics);

	bool checkTileCollisions(const Rectangle& other) const;

	const std::vector<Rectangle>& getCollision();

private:
	std::string _mapName;
	
	Vector2 _size;
	Vector2 _tileSize;
	
	SDL_Texture* _backgroundTexture;

	std::vector<Tile> _tileList;
	std::vector<Tileset> _tilesets;
	std::vector<Rectangle> _collisionRects;
	
	/* void loadMap
	 * Loads a map
	 */
	void loadMap(std::string mapName, Graphics& graphics);

};

class Stage {
public:
	Stage() {};
	Stage(std::vector<std::string> maps, Vector2 spawnPoint, Graphics& graphics);
	~Stage();
	void update(int elapsedTime, bool& isMoving);
	void draw(Graphics& graphics);

	/* bool checkTileCollisions
	*  returns true if there are collisions
	*/
	bool checkTileCollisions(const Rectangle& other) const;

	const Vector2 getPlayerSpawnPoint() const;

	const std::vector<Rectangle>& getCollision();
	
	void nextLevel(bool& isMoving, Ticket& ticket, Player& p, std::vector<Moveable>& moveables, bool isUndoable=true);
	
	void prevLevel(bool& isMoving, Ticket& ticket, Player& p, std::vector<Moveable>& moveables, bool isUndoable=true);
	
	
	void addFx(AnimatedSprite* fx);

	void undo(int ticketNum, bool& isMoving);

	void storeCurrState(int ticket, bool shouldMoveLeft);
	
private:
	std::vector<Level> _levels;
	std::vector<AnimatedSprite*> _fx;
	Vector2 _spawnPoint;
	int _idx=0;
	int _next=0;
	int _alpha=255;
	double _timeElapsed=0;
	double _timeToUpdate=0;
	std::stack<std::tuple<int, std::string>> _prevstates; //  ticket, function

	void prevLevel(bool& isMoving);
	void nextLevel(bool& isMoving);
};



#endif
