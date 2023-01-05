#ifndef LEVEL_H
#define LEVEL_H

#include <string>
#include <vector>
#include <stack>
#include <memory>

#include "globals.h"
#include "tile.h"
#include "rectangle.h"
#include "animatedsprite.h"
#include "tiletype.h"
#include "savestate.h"
#include "audio.h"



class Graphics;
class Moveable;
class Player;
struct SDL_Texture;

struct Tileset {
	std::shared_ptr<SDL_Texture> Texture;
	int FirstGid;
	Tileset() {
		this->FirstGid = -1;
	}
	Tileset(std::shared_ptr<SDL_Texture> texture, int firstGid) {
		this->Texture = texture;
		this->FirstGid = firstGid;
	}
};

class Level {
public:
	Level();
	Level(std::string mapName, Graphics& graphics);
	~Level();

	// copy constructor and operator
	Level(const Level& t);
	Level& operator=(const Level& t) noexcept;

	// move constructor and operator
	Level(Level&& t) noexcept;
	Level& operator=(Level&& t) noexcept;

	void update(int elapsedTime, const int& alpha);
	void draw(Graphics& graphics);

	bool checkTileCollisions(const Rectangle& other) const;

	const std::vector<Rectangle>& getCollision();

private:
	std::string _mapName;
	
	Vector2 _size;
	Vector2 _tileSize;
	
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
	Stage(std::vector<std::string> maps, Graphics& graphics);
	~Stage();

	// copy constructor and operator
	Stage(const Stage& t)=delete;
	Stage& operator=(const Stage& t)=delete;

	// move constructor and operator
	Stage(Stage&& t) noexcept;
	Stage& operator=(Stage&& t) noexcept;

	void update(int elapsedTime, bool& isMoving, Graphics& graphics);
	void draw(Graphics& graphics);

	/* bool checkTileCollisions
	*  returns true if there are collisions
	*/
	bool checkTileCollisions(const Rectangle& other) const;

	const Vector2 getPlayerSpawnPoint() const;

	const std::vector<Rectangle>& getCollision();
	
	void nextLevel(bool& isMoving, Ticket& ticket, Player& p, std::vector<Moveable>& moveables, bool isUndoable=true);
	
	void prevLevel(bool& isMoving, Ticket& ticket, Player& p, std::vector<Moveable>& moveables, bool isUndoable=true);
	
	
	void addFx(std::unique_ptr<AnimatedSprite>&& fx);

	void undo(int ticketNum, bool& isMoving);

	void storeCurrState(int ticket, int savedIdx);

	void restart(int ticket);

	void loadElements(std::string, Graphics& graphics);

	bool areAllMoveablesVisible();
	
	bool hasPlayerReachedEndPoint(Player& player);
	
	std::vector<Moveable>& getMoveables();

private:
	std::vector<Level> _levels;
	std::vector<std::unique_ptr<AnimatedSprite>> _fx;
	
	int _idx=0;
	int _next=0;
	int _alpha=255;
	double _timeElapsed=0;
	double _timeToUpdate=0;
	LimitedStack<std::tuple<int, int>> _prevstates; //  ticket, function

	Vector2 _spawnPoint;
	std::vector<Vector2> _moveableSpawnPoints;
	std::vector<Moveable> _moveables;
	std::vector<EndPointSprite> _endpoint;

	void prevLevel(bool& isMoving);
	void nextLevel(bool& isMoving);
};

class Overworld {
public:
	Overworld();
	Overworld(Vector2 spawnPoint, Graphics& graphics, std::unordered_map<std::string, std::string>& dialogueData);
	~Overworld();

	// copy constructor and operator
	Overworld(const Overworld& t) = delete;
	Overworld& operator=(const Overworld& t) = delete;

	// move constructor and operator
	Overworld(Overworld&& t) noexcept;
	Overworld& operator=(Overworld&& t) noexcept;

	void update(int elapsedTime, bool& isMoving, Graphics& graphics);
	void draw(Graphics& graphics);
	void setZoneCompleted(int zonenumber);
	
	int getZoneMapValue(int key);
	void save();
	
	Stage& getStage();

private:
	std::vector<CompletionSprite> _completionSprites;
	Stage _overworld;
	SaveState _save;
	std::unordered_map<int, int> _overworldzone_map;
};

#endif
