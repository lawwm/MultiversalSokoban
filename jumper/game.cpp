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
	
	this->_zone = Zone(globals::data, graphics, 0);
	this->_player = Player(graphics, this->_zone.getSpawnPoint());
	
	int LAST_UPDATE_TIME = SDL_GetTicks64();
	//Start the game loop
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
		if (input.isKeyHeld(SDL_SCANCODE_P) && input.wasKeyPressed(SDL_SCANCODE_P)) {
			std::cout << "meme" << std::endl;
			this->_zone.nextZone(graphics);
		}

		if (input.isKeyHeld(SDL_SCANCODE_Z) && input.wasKeyPressed(SDL_SCANCODE_Z) && _canPlayerMove && _canPlayerSwitchStage) {
			this->undo();
		}
		
		if (_canPlayerMove && _canPlayerSwitchStage && this->_player.getVisible()) {
			
			if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
				this->_player.moveLeft(this->_canPlayerMove, this->_zone.getStage(), this->_zone.getMoveables(), this->_ticket);
			}
			else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
				this->_player.moveRight(this->_canPlayerMove, this->_zone.getStage(), this->_zone.getMoveables(), this->_ticket);
			}
			else if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
				this->_player.moveUp(this->_canPlayerMove, this->_zone.getStage(), this->_zone.getMoveables(), this->_ticket);
			}
			else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
				this->_player.moveDown(this->_canPlayerMove, this->_zone.getStage(), this->_zone.getMoveables(), this->_ticket);
			}
			else if (input.isKeyHeld(SDL_SCANCODE_A) && input.wasKeyPressed(SDL_SCANCODE_A)) {
				this->_zone.getStage().prevLevel(this->_canPlayerSwitchStage, this->_ticket, this->_player, this->_zone.getMoveables());
			}
			else if (input.isKeyHeld(SDL_SCANCODE_D) && input.wasKeyPressed(SDL_SCANCODE_D)) {
				this->_zone.getStage().nextLevel(this->_canPlayerSwitchStage, this->_ticket, this->_player, this->_zone.getMoveables());
			}
			
			if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)
				&& !input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_DOWN)) {
				this->_player.stopMoving();
			}			
		}
		
		const int CURRENT_TIME_MS = SDL_GetTicks();
		int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
		this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
		LAST_UPDATE_TIME = CURRENT_TIME_MS;

		this->draw(graphics);
	}
}

void Game::draw(Graphics& graphics) {
	graphics.clear();

	this->_zone.draw(graphics);
	this->_player.draw(graphics);

	graphics.flip();
}

void Game::update(float elapsedTime, Graphics& graphics) {
	
	this->_player.update(elapsedTime, _canPlayerMove, this->_zone.getStage(), graphics, this->_canPlayerSwitchStage);
	this->_zone.update(elapsedTime, graphics, this->_canPlayerSwitchStage);
}

void Game::undo() {
	int retrievedTicket = this->_ticket.pollTicket();
	if (!retrievedTicket) return;
	
	this->_zone.undo(retrievedTicket, this->_canPlayerSwitchStage);
	this->_player.undo(retrievedTicket);
}
