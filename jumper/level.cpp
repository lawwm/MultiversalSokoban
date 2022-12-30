#include "level.h"
#include "graphics.h"
#include "globals.h"

#include "tinyxml2.h"

#include "SDL.h"

#include <sstream>
#include <algorithm>
#include <cmath>
#include <iostream>

using namespace tinyxml2;

Level::Level() {}

Level::Level(std::string mapName, Vector2 spawnPoint, Graphics& graphics) :
	_mapName(mapName),
	_size(Vector2(0, 0))
{
	this->loadMap(mapName, graphics);
}

Level::~Level() {

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
	if (pTileset != NULL) {
		while (pTileset) {
			int firstgid;
			const char* source = pTileset->FirstChildElement("image")->Attribute("source");
			char* path;
			std::stringstream ss;
			ss << source;
			pTileset->QueryIntAttribute("firstgid", &firstgid);
			SDL_Texture* tex = SDL_CreateTextureFromSurface(graphics.getRenderer(), graphics.loadImage(ss.str()));
			this->_tilesets.push_back(Tileset(tex, firstgid));

			pTileset = pTileset->NextSiblingElement("tileset");
		}
	}

	//Loading the layers
	XMLElement* pLayer = mapNode->FirstChildElement("layer");
	if (pLayer != NULL) {
		while (pLayer) {
			//Loading the data element
			XMLElement* pData = pLayer->FirstChildElement("data");
			if (pData != NULL) {
				while (pData) {
					//Loading the tile element
					XMLElement* pTile = pData->FirstChildElement("tile");
					if (pTile != NULL) {
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
							SDL_QueryTexture(tls.Texture, NULL, NULL, &tilesetWidth, &tilesetHeight);
							int tsxx = gid % (tilesetWidth / tileWidth) - 1;
							tsxx *= tileWidth;
							int tsyy = gid / (tilesetWidth / tileWidth);
							tsyy *= tileHeight;
							Vector2 finalTilesetPosition = Vector2(tsxx, tsyy);

							//Build the actual tile and add it to the level's tile list
							Tile tile(tls.Texture, Vector2(tileWidth, tileHeight),
								finalTilesetPosition, finalTilePosition);
							this->_tileList.push_back(tile);
							tileCounter++;

							pTile = pTile->NextSiblingElement("tile");
						}
					}

					pData = pData->NextSiblingElement("data");
				}
			}

			pLayer = pLayer->NextSiblingElement("layer");
		}
	}

	//Parse out the collisions
	XMLElement* pObjectGroup = mapNode->FirstChildElement("objectgroup");
	if (pObjectGroup != NULL) {
		while (pObjectGroup) {
			const char* name = pObjectGroup->Attribute("name");
			std::stringstream ss;
			ss << name;
			if (ss.str() == "collisions") {
				XMLElement* pObject = pObjectGroup->FirstChildElement("object");
				if (pObject != NULL) {
					while (pObject) {
						float x, y, width, height;
						x = pObject->FloatAttribute("x");
						y = pObject->FloatAttribute("y");
						width = pObject->FloatAttribute("width");
						height = pObject->FloatAttribute("height");
						// make the rectangle one unit smaller from each side so that player 
						// character can move within tight spaces
						this->_collisionRects.push_back(Rectangle(
							std::ceil(x) * globals::SPRITE_SCALE,
							std::ceil(y) * globals::SPRITE_SCALE,
							std::ceil(width) * globals::SPRITE_SCALE,
							std::ceil(height) * globals::SPRITE_SCALE
						));

						pObject = pObject->NextSiblingElement("object");
					}
				}
			}

			pObjectGroup = pObjectGroup->NextSiblingElement("objectgroup");
		}
	}

}

void Level::update(int elapsedTime, const int& alpha) {
	for (Tile& t : this->_tileList) {
		t.update(elapsedTime, alpha);
	}
}

void Level::draw(Graphics& graphics) {
	for (int i = 0; i < this->_tileList.size(); i++) {
		this->_tileList.at(i).draw(graphics);
	}
}

const std::vector<Rectangle>& Level::getCollision() {
	return this->_collisionRects;
};

bool Level::checkTileCollisions(const Rectangle& other) const {
	for (int i = 0; i < _collisionRects.size(); i++) {
		if (_collisionRects.at(i).collidesWith(other)) return false;
	}

	return true;
}

const Vector2 Stage::getPlayerSpawnPoint() const {
	return this->_spawnPoint;
}

Stage::Stage(std::vector<std::string> maps, Vector2 spawnPoint, Graphics& graphics) :
	_spawnPoint(spawnPoint)
{
	for (auto& map : maps) {
		this->_levels.push_back(Level(map, Vector2(160, 160), graphics));
	}
}

Stage::~Stage() {

}

void Stage::update(int elapsedTime, bool& isMoving) {
	this->_timeElapsed += elapsedTime;
	if (this->_timeElapsed > this->_timeToUpdate) {
		this->_timeElapsed -= this->_timeToUpdate;
		if (isMoving) {
			return;
		}

		if (this->_alpha > 0) { // fade out previous stage
			this->_levels[this->_idx].update(elapsedTime, this->_alpha);
		} else if (this->_alpha == 0) { // set current idx to next idx 
			this->_idx = this->_next; 
		} else { // fade in next stage
			this->_levels[this->_idx].update(elapsedTime, -1 * this->_alpha);
			if (this->_alpha == -255) {
				this->_alpha = 255;
				isMoving = true;
			}
		}
		
		this->_alpha -= 1;
		
	}
}

void Stage::draw(Graphics& graphics) {
	this->_levels[this->_idx].draw(graphics);
}

void Stage::nextLevel(bool& isMoving) {
	this->_next = (this->_idx + 1) % this->_levels.size();
	isMoving = 0;
}

void Stage::prevLevel(bool& isMoving) {
	std::cout << isMoving << std::endl;
	this->_next = (this->_idx - 1 + this->_levels.size()) % this->_levels.size();
	isMoving = 0;
}

const std::vector<Rectangle>& Stage::getCollision() {
	return this->_levels[this->_idx].getCollision();
};

bool Stage::checkTileCollisions(const Rectangle& other) const {
	return this->_levels[this->_idx].checkTileCollisions(other);
}