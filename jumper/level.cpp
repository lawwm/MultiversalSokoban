#include "level.h"


using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Graphics& graphics) :
	_mapName(mapName),
	_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

}


Level::Level(const Level& t)
{
	std::cout << "level copy constructor" << std::endl;
	
	this->_mapName = t._mapName;
	this->_size = t._size;
	this->_tileSize = t._tileSize;

	this->_tileList = t._tileList;
	this->_tilesets = t._tilesets;
	this->_collisionRects = t._collisionRects;
	this->_poisonRects = t._poisonRects;
	
	this->_permeable = t._permeable;
}

Level& Level::operator=(const Level& t) noexcept
{
	//std::cout << "tile copy opeartor" << std::endl;

	this->_mapName = t._mapName;
	this->_size = t._size;
	this->_tileSize = t._tileSize;

	this->_tileList = t._tileList;
	this->_tilesets = t._tilesets;
	this->_collisionRects = t._collisionRects;
	this->_poisonRects = t._poisonRects;

	this->_permeable = t._permeable;
	
	return *this;
}

Level::Level(Level&& t) noexcept
{
	//std::cout << "tile move constructor" << std::endl;

	this->_mapName = std::move(t._mapName);
	this->_size = std::move(t._size);
	this->_tileSize = std::move(t._tileSize);

	this->_tileList = std::move(t._tileList);
	this->_tilesets = std::move(t._tilesets);
	this->_collisionRects = std::move(t._collisionRects);
	this->_poisonRects = std::move(t._poisonRects);
	this->_permeable = std::move(t._permeable);
	
	t._tileList.clear();
	t._tilesets.clear();
	t._collisionRects.clear();
	t._permeable.clear();
}

Level& Level::operator=(Level&& t) noexcept
{
	if (this == &t) return *this;

	this->_mapName = std::move(t._mapName);
	this->_size = std::move(t._size);
	this->_tileSize = std::move(t._tileSize);
	
	this->_tileList = std::move(t._tileList);
	this->_tilesets = std::move(t._tilesets);
	this->_collisionRects = std::move(t._collisionRects);
	this->_poisonRects = std::move(t._poisonRects);
	this->_permeable = std::move(t._permeable);
	
	t._tileList.clear();
	t._tilesets.clear();
	t._collisionRects.clear();
	t._permeable.clear();
	
	return *this;
}

void Level::loadMap(std::string mapName, Graphics& graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;
		
	ss << mapName << ".tmx"; //Pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	//Get the width and the height of the whole map and store it in _size
	int width, height;
	mapNode->QueryIntAttribute("width", &width);
	mapNode->QueryIntAttribute("height", &height);
	this->_size = Vector2(width, height);

	//Get the width and the height of the tiles and store it in _tileSize
	int tileWidth, tileHeight;
	mapNode->QueryIntAttribute("tilewidth", &tileWidth);
	mapNode->QueryIntAttribute("tileheight", &tileHeight);
	this->_tileSize = Vector2(tileWidth, tileHeight);


	//Loading the tilesets, tilesets is the image that shows the tiles that are available to be used in the map
	XMLElement* pTileset = mapNode->FirstChildElement("tileset");
	
	while (pTileset) {
		int firstgid;
		const char* source = pTileset->FirstChildElement("image")->Attribute("source");
		char* path;
		std::stringstream ss;
		ss << source;
		pTileset->QueryIntAttribute("firstgid", &firstgid);
		
		std::shared_ptr<SDL_Texture> tex(SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str())), [](SDL_Texture* texture) {
			std::cout << "destory texture" << std::endl;
			SDL_DestroyTexture(texture);
		});
		
		this->_tilesets.push_back(Tileset(tex, firstgid));

		pTileset = pTileset->NextSiblingElement("tileset");
	}
	

	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");

	while (pLayer) {
		const char* name = pLayer->Attribute("name");
		std::stringstream ss;
		ss << name;
		bool isDestructible = (ss.str() == "destructible");

		//Loading the data element
		XMLElement* pData = pLayer->FirstChildElement("data");
		while (pData) {
			//Loading the tile element
			XMLElement* pTile = pData->FirstChildElement("tile");

			int tileCounter = 0;
			while (pTile) {
				//Get the tileset for this specific gid
				int gid = pTile->IntAttribute("gid");
				Tileset tls;
				for (int i = 0; i < this->_tilesets.size(); i++) {
					if (this->_tilesets[i].FirstGid <= gid) {
						//This is the tileset we want
						tls = this->_tilesets.at(i);
						break;
					}
				}
							
				//If gid is 0, no tile should be drawn. Continue loop
				if (tls.FirstGid == -1 || gid == 0) {
					//No tileset was found for this gid
					tileCounter++;
					if (!pTile->NextSiblingElement("tile")) {
						break;
					}

					pTile = pTile->NextSiblingElement("tile");
					continue;
				}

				//Get the position of the tile in the level
				int xx = tileWidth * (tileCounter % width);
				int yy = tileHeight * (tileCounter / width);
				Vector2 finalTilePosition = Vector2(xx, yy);

				//Calculate the position of the tile in the tileset
				int tilesetWidth, tilesetHeight;
				SDL_QueryTexture(tls.Texture.get(), NULL, NULL, &tilesetWidth, &tilesetHeight);
				int tsxx = gid % (tilesetWidth / tileWidth) - 1;
				tsxx *= tileWidth;
				int tsyy = gid / (tilesetWidth / tileWidth);
				tsyy *= tileHeight;
				Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

				//Build the actual tile and add it to the level's tile list
				Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
					finalTilesetPosition, finalTilePosition, isDestructible);
				this->_tileList.push_back(std::move(tile));
				tileCounter++;

				pTile = pTile->NextSiblingElement("tile");
			}
			
			pData = pData->NextSiblingElement("data");
		}
		

		pLayer = pLayer->NextSiblingElement("layer");
	}

	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");

	while (pObjectGroup) {
		const char* name = pObjectGroup->Attribute("name");
		std::stringstream ss;
		ss << name;

		//Parse out the collisions
		if (ss.str() == "collisions" || ss.str() == "poison") {
			XMLElement* pObject = pObjectGroup->FirstChildElement("object");

			while (pObject) {
				float x, y, width, height;
				x = pObject->FloatAttribute("x");
				y = pObject->FloatAttribute("y");
				width = pObject->FloatAttribute("width");
				height = pObject->FloatAttribute("height");
				// make the rectangle one unit smaller from each side so that player 
				// character can move within tight spaces
				Rectangle rec(
					std::ceil(x)* globals::SPRITE_SCALE,
					std::ceil(y)* globals::SPRITE_SCALE,
					std::ceil(width)* globals::SPRITE_SCALE,
					std::ceil(height)* globals::SPRITE_SCALE
				);
				if (ss.str() == "collisions") {
					this->_collisionRects.push_back(rec);
				}
				else if (ss.str() == "poison") {
					this->_poisonRects.push_back(rec);
				}
				else {
					assert(false);
				}
				

				pObject = pObject->NextSiblingElement("object");
			}
		}

		pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
	}
	

	

	

}

void Level::storeCurrState(int ticket) {
	this->_prevstates.push({ ticket, this->_permeable });
}

void Level::update(int elapsedTime, const int& alpha) {
	for (Tile& t : this->_tileList) {
		t.update(elapsedTime, alpha);
	}
}

void Level::draw(Graphics& graphics) {
	for (Tile& t : this->_tileList) {
		int key = Tile::getScreenKeyValue(t.getPosition().x, t.getPosition().y);
		if (!t.isDestructible() || this->_permeable.find(key) == this->_permeable.end()) {
			t.draw(graphics);
		}
	}
}

const std::vector<Rectangle>& Level::getCollision() {
	return this->_collisionRects;
};

bool Level::checkTileCollisions(const Rectangle& other) const {
	int key = Tile::getScreenKeyValue(other.getLeft(), other.getTop());
	if (this->_permeable.find(key) != this->_permeable.end()) {
		return true;
	}

	for (int i = 0; i < _collisionRects.size(); i++) {
		if (_collisionRects.at(i).collidesWith(other)) {
			return false;
		}
	}
	return true;
}

bool Level::checkTilePoison(const Rectangle& other) const {
	for (int i = 0; i < this->_poisonRects.size(); i++) {
		if (this->_poisonRects.at(i).collidesWith(other)) return false;
	}

	return true;
}

void Level::addPermeable(Vector2 coor)
{
	// add permeable tile to the current level
	int key = Tile::getScreenKeyValue(coor.x, coor.y);
	this->_permeable[key]++;
}

void Level::undo(int ticketNum) {

	// switch dimension undo
	if (_prevstates.empty() || std::get<0>(this->_prevstates.top()) != ticketNum) return;

	this->_permeable = std::get<1>(this->_prevstates.top());
	this->_prevstates.pop();
}

void Level::restart(int ticketNum) {
	this->storeCurrState(ticketNum);
	this->_permeable = {};
}


const Vector2 Stage::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

Stage::Stage(std::vector<std::string> maps, Graphics& graphics)
{
	for (auto& map : maps) {
		this->_levels.push_back(std::move(Level(map, graphics)));
	}
	
	this->loadElements(maps[0], graphics);
}

Stage::~Stage() {

}

Stage::Stage(Stage&& t) noexcept
{
	//std::cout << "tile move constructor" << std::endl;

	this->_levels = std::move(t._levels);
	this->_fx = std::move(t._fx);
	
	this->_idx = t._idx;
	this->_next = t._next;
	this->_alpha = t._alpha;
	this->_timeElapsed = t._timeElapsed;
	this->_timeToUpdate = t._timeToUpdate;
	
	this->_prevstates = t._prevstates;
	this->_spawnPoint = t._spawnPoint;
	this->_moveableSpawnPoints = std::move(t._moveableSpawnPoints);
	this->_moveables = std::move(t._moveables);
	this->_endpoint = std::move(t._endpoint);

	t._levels.clear();
	t._fx.clear();
	t._moveableSpawnPoints.clear();
	t._moveables.clear();
	t._endpoint.clear();

}

Stage& Stage::operator=(Stage&& t) noexcept
{
	if (this == &t) return *this;

	this->_levels = std::move(t._levels);
	this->_fx = std::move(t._fx);

	this->_idx = t._idx;
	this->_next = t._next;
	this->_alpha = t._alpha;
	this->_timeElapsed = t._timeElapsed;
	this->_timeToUpdate = t._timeToUpdate;

	this->_prevstates = t._prevstates;
	this->_spawnPoint = t._spawnPoint;
	this->_moveableSpawnPoints = std::move(t._moveableSpawnPoints);
	this->_moveables = std::move(t._moveables);
	this->_endpoint = std::move(t._endpoint);

	t._levels.clear();
	t._fx.clear();
	t._moveableSpawnPoints.clear();
	t._moveables.clear();
	t._endpoint.clear();
	
	return *this;
}


void Stage::loadElements(std::string mapName, Graphics& graphics) {
	//Parse the .tmx file
	XMLDocument doc;
	std::stringstream ss;

	ss << mapName << ".tmx"; //Pass in Map 1, we get maps/Map 1.tmx
	doc.LoadFile(ss.str().c_str());

	XMLElement* mapNode = doc.FirstChildElement("map");

	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "spawnpoint") {
				// parse out the spawn point
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y;
						x = pObject->FloatAttribute("x") * globals::SPRITE_SCALE;
						y = pObject->FloatAttribute("y") * globals::SPRITE_SCALE;

						// add to spawn
						std::cout << "soy " << x << " " << y << std::endl;
						this->_spawnPoint = Vector2(x, y);
						
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "endpoint") {
				// parse out the endstate
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y;
						bool shouldShow = true;
						x = pObject->FloatAttribute("x") * globals::SPRITE_SCALE;
						y = pObject->FloatAttribute("y") * globals::SPRITE_SCALE;

						// check properties for whether the chest has been completed
						XMLElement* pProperties = pObjectGroup->FirstChildElement("properties");
						while (pProperties) {
							XMLElement* pProperty = pProperties->FirstChildElement("property");
							while (pProperty) {
								const char* name = pProperty->Attribute("name");
								std::stringstream ss;
								ss << name;
								if (ss.str() == "visible") {
									shouldShow = pProperty->BoolAttribute("value");
								}
								pProperty = pProperty->NextSiblingElement("property");
							}
							pProperties = pProperties->NextSiblingElement("properties");
						}

						// add to endpoint
						this->_endpoint.push_back(EndPointSprite(graphics, Vector2(x, y), shouldShow));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			else if (ss.str() == "coins" || ss.str() == "sushi" || ss.str() == "bomb") {
				// parse out coin positions
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y;
						x = pObject->FloatAttribute("x") * globals::SPRITE_SCALE;
						y = pObject->FloatAttribute("y") * globals::SPRITE_SCALE;

						// add to moveables
						if (ss.str() == "coins") {
							this->_moveables.push_back(new Coin(graphics, Vector2(x, y)));
						}
						else if (ss.str() == "sushi") {
							this->_moveables.push_back(new Sushi(graphics, Vector2(x, y)));
						}
						else if (ss.str() == "bomb") {
							this->_moveables.push_back(new Bomb(graphics, Vector2(x, y)));
						}
						else {
							assert(false);
						}
						this->_moveableSpawnPoints.push_back(Vector2(x, y));
						pObject = pObject->NextSiblingElement("object");
					}
				}
			}
			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}
}

bool Stage::hasPlayerWon(Player& player)
{
	// check that player is stationary and not invisible
	if (!player.isStationary() || !player.getVisible()) {
		return false;
	}

	// check that each check point has collided with a coin
	for (auto& endpointSprite : this->_endpoint) {
		bool hasCollisionWithCoinOrPlayer = player.getBoundingBox().collidesWith(endpointSprite.getBoundingBox());
		for (auto& moveable : this->_moveables) {
			hasCollisionWithCoinOrPlayer |= moveable->collidesWith(endpointSprite.getBoundingBox());
			if (hasCollisionWithCoinOrPlayer) continue;
		}

		if (!hasCollisionWithCoinOrPlayer) return false;
	}

	// check that no coins are destroyed and all sushi are destroyed, i.e. they are in a winnable state
	for (auto& moveable : this->_moveables) {
		if (!moveable->hasItWon()) return false;
	}
	
	return true;
}


bool Stage::isItPossibleToWin()
{
	for (Moveable* movable : this->_moveables) {
		if (!movable->isItPossibleToWin()) {
			return false;
		}
	}
	return true;
}


std::vector<Moveable*>& Stage::getMoveables()
{
	return this->_moveables;
}

void Stage::update(int elapsedTime, bool& isMoving, Graphics& graphics) {
	this->_timeElapsed += elapsedTime;
	
	// update the special effects
	for (auto itr = _fx.begin(); itr != _fx.end();) {
		if ((*itr)->getVisible()) {
			(*itr)->update(elapsedTime);
			itr++;
			continue;
		}
		// clean up the effect
		itr = _fx.erase(itr);
	}
	
	// update the moveables
	for (auto& m : _moveables) {
		m->update(elapsedTime, *this, graphics, isMoving);
	}

	// update the endpoint
	for (auto& ep : this->_endpoint) {
		ep.update(elapsedTime);
	}

	// Update the transition between dimensions
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		//std::cout << _fx.size() << std::endl;
		
		if (isMoving) {
			return;
		}

		// for transition when switching levels
		if (this->_alpha > 0) { // fade out previous stage
			this->_levels[this->_idx].update(elapsedTime, this->_alpha);
		} else if (this->_alpha == 0) { // set current idx to next idx 
			this->_idx = this->_next; 
		} else { // fade in next stage
			this->_levels[this->_idx].update(elapsedTime, -1 * this->_alpha);
			if (this->_alpha <= -255) {
				this->_alpha = 255;
				isMoving = true;
			}
		}
		
		this->_alpha -= 1;
		
	}
}

void Stage::draw(Graphics& graphics) {
	this->_levels[this->_idx].draw(graphics);

	for (auto& ptr : _fx) {
		//std::cout << &(this->_fx[i]) << std::endl;
		ptr->draw(graphics);
	}

	for (auto& m : this->_moveables) {
		m->draw(graphics);
	}

	for (auto& ep : this->_endpoint) {
		ep.draw(graphics);
	}
	
}

void Stage::nextLevel(bool& isMoving, Ticket& ticket, Player& player, std::vector<Moveable*>& moveables, bool isUndoable) {
	// prevent switching stages when there is only one stage
	if (this->_levels.size() == 1) return;
	
	if (isUndoable) {
		int generatedTicket = ticket.insertTicket();
		this->storeCurrState(generatedTicket, this->_idx);
		player.storeCurrState(generatedTicket);
		for (auto& moveable : moveables) {
			moveable->storeCurrState(generatedTicket);
		}
	}

	this->nextLevel(isMoving);
}

void Stage::nextLevel(bool& isMoving)
{
	this->_next = (this->_idx + 1) % this->_levels.size();
	Foley::playSound("transition");
	isMoving = false;
}

void Stage::prevLevel(bool& isMoving, Ticket& ticket, Player& player, std::vector<Moveable*>& moveables, bool isUndoable) {
	// prevent switching stages when there is only one stage
	if (this->_levels.size() == 1) return;
	
	if (isUndoable) {
		int generatedTicket = ticket.insertTicket();
		this->storeCurrState(generatedTicket, this->_idx);
		player.storeCurrState(generatedTicket);
		for (auto& moveable : moveables) {
			moveable->storeCurrState(generatedTicket);
		}
	}

	this->prevLevel(isMoving);
}

void Stage::prevLevel(bool& isMoving)
{
	this->_next = (this->_idx - 1 + this->_levels.size()) % this->_levels.size();
	Foley::playSound("transition");
	isMoving = false;
}

void Stage::addFx(std::unique_ptr<AnimatedSprite>&& fx)
{
	this->_fx.push_back(std::move(fx));
}

const std::vector<Rectangle>& Stage::getCollision() {
	return this->_levels[this->_idx].getCollision();
};

bool Stage::checkTileCollisions(const Rectangle& other) const {
	return this->_levels[this->_idx].checkTileCollisions(other);
}

bool Stage::checkTilePoison(const Rectangle& other) const {
	return this->_levels[this->_idx].checkTilePoison(other);
}

void Stage::undo(int ticketNum, bool& isMoving)
{
	// undo moveables
	for (auto& m : this->_moveables) {
		m->undo(ticketNum);
	}
	

	// undo level
	for (auto& level : this->_levels) {
		level.undo(ticketNum);
	}

	// switch dimension undo
	if (_prevstates.empty() || std::get<0>(_prevstates.top()) != ticketNum) return;
	
	auto [ticketNumber, x] = this->_prevstates.top();
	this->_prevstates.pop();
	this->_idx = x;
	this->_next = x;
	this->_levels[this->_idx].update(0, this->_alpha);
}

void Stage::storeCurrState(int ticket, int savedIdx)
{
	this->_prevstates.emplace(ticket, savedIdx);
	for (auto& level : this->_levels) {
		level.storeCurrState(ticket);
	}
}

void Stage::restart(int generatedTicket)
{
	// restart dimension position
	this->storeCurrState(generatedTicket, this->_idx);
	this->_idx = 0;
	this->_next = 0;
	this->_levels[this->_idx].update(0, this->_alpha);

	// restart level
	for (auto& level : this->_levels) {
		level.restart(generatedTicket);
	}

	// restart moveables position
	for (int i = 0; i < _moveables.size(); ++i) {
		this->_moveables[i]->restart(_moveableSpawnPoints[i], generatedTicket);
	}
}

void Stage::addPermeable(Vector2 coor)
{
	// add permeable tile to the current level
	this->_levels[this->_idx].addPermeable(coor);
}


Overworld::Overworld()
{
}

Overworld::Overworld(Vector2 spawnPoint, Graphics& graphics, std::unordered_map<std::string, std::string>& dialogueData)
{
	this->_overworld = std::move(Stage({ globals::overworld }, graphics));

	// initialise chest at level select area from xml file
	this->_save.parse(globals::overworld, this->_completionSprites, graphics);
	
	std::cout << "how many ocmpletion sprites" << this->_completionSprites.size() << std::endl;
	
	// fill out coordinates map
	for (auto& completionSprite : this->_completionSprites) {

		// fill out coordinates map
		int key = completionSprite.getX() + completionSprite.getY() * globals::SCREEN_WIDTH;
		this->_overworldzone_map.insert({ key, completionSprite.getLevelNumber() });
	
		// insert into the dialogueData
		std::string stagenumber = std::to_string(completionSprite.getLevelNumber());
		std::string prompt = "Press A to proceed to Stage " + stagenumber + ".";
		dialogueData[stagenumber] = prompt;
	}
}

Overworld::~Overworld()
{
}


Overworld::Overworld(Overworld&& t) noexcept
{
	this->_completionSprites = std::move(t._completionSprites);
	this->_overworld = std::move(t._overworld);
	this->_save = std::move(t._save);
	this->_overworldzone_map = std::move(t._overworldzone_map);

	t._overworldzone_map.clear();
}

Overworld& Overworld::operator=(Overworld&& t) noexcept
{
	if (this == &t) return *this;

	this->_completionSprites = std::move(t._completionSprites);
	this->_overworld = std::move(t._overworld);
	this->_save = std::move(t._save);
	this->_overworldzone_map = std::move(t._overworldzone_map);

	t._completionSprites.clear();
	t._overworldzone_map.clear();
	return *this;
}

void Overworld::update(int elapsedTime, bool& isMoving, Graphics& graphics)
{
	this->_overworld.update(elapsedTime, isMoving, graphics);
	for (CompletionSprite& completionSprite : this->_completionSprites) {
		completionSprite.update(elapsedTime);
	}
}

void Overworld::draw(Graphics& graphics)
{
	this->_overworld.draw(graphics);
	
	for (CompletionSprite& completionSprite : this->_completionSprites) {
		//std::cout << completionSprite.getX() << " " << completionSprite.getY() << std::endl;
		completionSprite.draw(graphics);
	}
}

Stage& Overworld::getStage()
{
	return this->_overworld;
}

void Overworld::setZoneCompleted(int zonenumber)
{
	zonenumber++;
	for (CompletionSprite& completionSprite : this->_completionSprites) {
		if (completionSprite.getLevelNumber() == zonenumber) {
			std::cout << completionSprite.getLevelNumber() << std::endl;
			completionSprite.setCompleted(true);
		}
	}
}

int Overworld::getZoneMapValue(int key) // return 0 if cannot be found
{
	if (this->_overworldzone_map.find(key) == this->_overworldzone_map.end()) {
		return 0;
	}
	
	return this->_overworldzone_map[key];
}

void Overworld::save()
{
	this->_save.update(globals::overworld, this->_completionSprites);
}
