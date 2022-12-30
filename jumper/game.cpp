#include "SDL.h"

#include "game.h"
#include "graphics.h"
#include "input.h"
#include "globals.h"
#include "moveable.h"
#include <iostream>
/* Game class
 * This class holds all information for our main game loop
 */

namespace {
	const int FPS = 50;
	const int MAX_FRAME_TIME = 5 * 1000 / FPS;
}

Game::Game() {
	SDL_Init(SDL_INIT_EVERYTHING);
	this->gameLoop();
}

Game::~Game() {

}

void Game::gameLoop() {
	Graphics graphics;
	Input input;
	SDL_Event event;

	this->_level = Level(globals::iceLevel, Vector2(160, 160), graphics);
	this->_player = Player(graphics, this->_level.getPlayerSpawnPoint());
	this->_moveables.push_back(Moveable(graphics, Vector2(128, 128)));
	this->_moveables.push_back(Moveable(graphics, Vector2(288, 96)));
	this->_moveables.push_back(Moveable(graphics, Vector2(192, 128)));
	this->_moveables.push_back(Moveable(graphics, Vector2(160, 128)));
	int LAST_UPDATE_TIME = SDL_GetTicks64();
	//Start the game loopj
	while (true) {
		input.beginNewFrame();

		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_KEYDOWN) {
				if (event.key.repeat == 0) {
					input.keyDownEvent(event);
				}
			}
			else if (event.type == SDL_KEYUP) {
				input.keyUpEvent(event);
			}
			else if (event.type == SDL_QUIT) {
				return;
			}
		}
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) == true) {
			return;
		}
		
		if (_canPlayerMove) {
			if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
				this->_player.moveLeft(this->_canPlayerMove, this->_level.getCollision());
			}
			else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
				this->_player.moveRight(this->_canPlayerMove, this->_level.getCollision());
			}
			else if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
				this->_player.moveUp(this->_canPlayerMove, this->_level.getCollision());
			}
			else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
				this->_player.moveDown(this->_canPlayerMove, this->_level.getCollision());
			}

			if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)
				&& !input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_DOWN)) {
				this->_player.stopMoving();
			}			
		}
		
		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME));
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics& graphics) {
	graphics.clear();

	this->_level.draw(graphics);
	this->_player.draw(graphics);
	for (auto& m : _moveables) {
		m.draw(graphics);
	}
	graphics.flip();
}

void Game::update(float elapsedTime) {
	this->_player.update(elapsedTime, _canPlayerMove);
	this->_level.update(elapsedTime);
	for (auto& m : _moveables) {
		m.update(elapsedTime);
	}
	//Check collisions
	//std::vector<Rectangle> others = this->_level.checkTileCollisions(this->_player.getBoundingBox());
	//
	//if (others.size() > 0) {
	//	//Player collided with at least one tile. Handle it.
	//	this->_player.handleTileCollisions(others);
	//}
}
