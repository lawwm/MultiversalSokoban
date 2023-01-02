#include "SDL.h"

#include "game.h"
#include "graphics.h"
#include "input.h"
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
	
	std::unordered_map<std::string, std::string> dialogueData({
		{globals::died_dialogue, "You lost. Press Z to undo or R to restart"},
		{globals::won_dialogue, "You won. Press N to proceed."},
		{globals::exit_dialogue, "Press Esc again to confirm exit.\nPress A to continue playing.\nPress D for level select."},
		{globals::overworld_exit_dialogue, "Press Esc again to confirm exit.\nPress A to continue playing."},
	});

	this->_zone = Zone(globals::data, graphics, 0);
	this->_player = Player(graphics, Vector2(globals::overworld_spawn_x, globals::overworld_spawn_y));
	this->_overworld = Overworld(Vector2(256, 256), graphics, dialogueData);
	
	this->_openingScreen = OpeningScreen(graphics);
	this->_victoryScreen = VictoryScreen(graphics);
	
	this->_textbox = TextBox(graphics, dialogueData); // need to insert last as it uses the dialogueData
	
	int LAST_UPDATE_TIME = SDL_GetTicks64();
	//Start the game loop
	while (true) {
		input.beginNewFrame();

		// store key register data
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

		if (this->_currScreen == ZONE) {
			if (!individualZone(graphics, input, LAST_UPDATE_TIME)) {
				return;
			}
		}
		else if (this->_currScreen == OVERWORLD) {
			if (!overworld(graphics, input, LAST_UPDATE_TIME)) {
				return;
			}
		}
		else if (this->_currScreen == START_SCREEN) {
			if (!openingscreen(graphics, input, LAST_UPDATE_TIME)) {
				return;
			}
		}
		else if (this->_currScreen == VICTORY_SCREEN) {
			if (!victoryscreen(graphics, input, LAST_UPDATE_TIME)) {
				return;
			}
		}

	}
}



void Game::draw(Graphics& graphics) {
	graphics.clear();

	if (this->_currScreen == OVERWORLD) {
		this->_overworld.draw(graphics);
		this->_player.draw(graphics);
		this->_textbox.draw(graphics);
	}
	else if (this->_currScreen == ZONE) {
		this->_zone.draw(graphics);
		this->_player.draw(graphics);	
		this->_textbox.draw(graphics);
	}
	else if (this->_currScreen == START_SCREEN) {
		this->_openingScreen.draw(graphics);
	}
	else if (this->_currScreen == VICTORY_SCREEN) {
		this->_victoryScreen.draw(graphics);
	}
	
	
	graphics.flip();
}

void Game::update(float elapsedTime, Graphics& graphics) {
	
	if (this->_currScreen == OVERWORLD) {
		this->_player.update(elapsedTime, _canPlayerMove, this->_overworld.getStage(), graphics, this->_canPlayerSwitchStage);
		this->_overworld.update(elapsedTime, this->_canPlayerSwitchStage);
	}
	else if (this->_currScreen == ZONE) {
		this->_player.update(elapsedTime, _canPlayerMove, this->_zone.getStage(), graphics, this->_canPlayerSwitchStage);
		this->_zone.update(elapsedTime, graphics, this->_canPlayerSwitchStage);

		// display undo dialogue text if player has died
		if (this->_player.getVisible() == false || !this->_zone.areAllMoveablesVisible()) {
			this->_textbox.set(globals::died_dialogue);
		}		
	}
	else if (this->_currScreen == START_SCREEN) {
		this->_openingScreen.update(elapsedTime, graphics);
	}
	else if (this->_currScreen == VICTORY_SCREEN) {
		this->_victoryScreen.update(elapsedTime, graphics);
	}
}

void Game::undo() {
	int retrievedTicket = this->_ticket.pollTicket();
	if (!retrievedTicket) return;
	
	this->_zone.undo(retrievedTicket, this->_canPlayerSwitchStage);
	this->_player.undo(retrievedTicket);
}

void Game::restart(Graphics& graphics)
{
	int insertTicket = this->_ticket.insertTicket();
	Vector2 spawn = this->_zone.getSpawnPoint();
	this->_zone.restart(graphics, insertTicket);
	this->_player.restart(spawn, insertTicket);
}


bool Game::individualZone(Graphics& graphics, Input& input, int& LAST_UPDATE_TIME) // true to continue, false to return
{
	// leave the game
	if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) || this->_textbox.getKey() == globals::exit_dialogue) {
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) && this->_textbox.getKey() == globals::exit_dialogue) {
			return false;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_A)) { // close menu
			this->_textbox.clearDialogue();
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_D)) {
			this->_textbox.clearDialogue();
			this->_player = Player(graphics, Vector2(globals::overworld_spawn_x, globals::overworld_spawn_y));
			this->_currScreen = OVERWORLD;
		}
		else if (this->_textbox.getKey() != globals::exit_dialogue) { // first clicking exit
			this->_textbox.set(globals::exit_dialogue);
			this->draw(graphics);
		}
		return true;
	}


	// check whether this player has won
	bool hasPlayerWon = this->_zone.areAllMoveablesVisible() && this->_zone.hasPlayerReachedEndPoint(this->_player);
	if (hasPlayerWon) {
		
		// set zone to completed in overworld
		if (this->_textbox.getKey() != globals::won_dialogue) {
			this->_textbox.set(globals::won_dialogue);
			this->_overworld.setZoneCompleted(this->_zone.getZoneNumber());
			this->_overworld.save();
		}

		
		
		// if player decides to skip to next screen
		if (input.isKeyHeld(SDL_SCANCODE_N) && input.wasKeyPressed(SDL_SCANCODE_N)) {
			this->_zone.nextZone(graphics, this->_currScreen);
			this->_textbox.clearDialogue();
			return true;
		}
	}

	// undo or restart
	if (_canPlayerMove && _canPlayerSwitchStage && !hasPlayerWon) {
		if (input.isKeyHeld(SDL_SCANCODE_Z) && input.wasKeyPressed(SDL_SCANCODE_Z)) {
			this->undo();
			if (this->_textbox.getKey() == globals::died_dialogue) {
				this->_textbox.clearDialogue();
			}
		}
		else if (input.isKeyHeld(SDL_SCANCODE_R) && input.wasKeyPressed(SDL_SCANCODE_R)) {
			this->restart(graphics);
			if (this->_textbox.getKey() == globals::died_dialogue) {
				this->_textbox.clearDialogue();
			}
		}

	}


	// movement and switching dimension
	if (_canPlayerMove && _canPlayerSwitchStage && this->_player.getVisible() && !hasPlayerWon && this->_zone.areAllMoveablesVisible()) {

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



	const int CURRENT_TIME_MS = SDL_GetTicks64();
	int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
	this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
	LAST_UPDATE_TIME = CURRENT_TIME_MS;

	this->draw(graphics);

	return true;
}

bool Game::overworld(Graphics& graphics, Input& input, int& LAST_UPDATE_TIME)
{
	std::vector<Moveable> empty;
	// leave the game
	if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) || this->_textbox.getKey() == globals::overworld_exit_dialogue) {
		if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE) && this->_textbox.getKey() == globals::overworld_exit_dialogue) {
			return false;
		}
		else if (input.wasKeyPressed(SDL_SCANCODE_A)) { // close menu
			this->_textbox.clearDialogue();
		}
		else if (this->_textbox.getKey() != globals::overworld_exit_dialogue) { // first clicking exit
			this->_textbox.set(globals::overworld_exit_dialogue);
			this->draw(graphics);
		}
		return true;
	}
	
	// level select
	float key = this->_player.getY() * globals::SCREEN_WIDTH + this->_player.getX();

	if (this->_overworld.getZoneMapValue(key)) {
		int value = this->_overworld.getZoneMapValue(key);
		if (this->_isPlayerInLevelSelect == 0) {
			this->_isPlayerInLevelSelect = 1;
			this->_textbox.set(std::to_string(value));
			this->draw(graphics);
			return true;
		}
		else if (this->_isPlayerInLevelSelect == 1) {
			if (input.wasKeyPressed(SDL_SCANCODE_A)) {
				this->_currScreen = ZONE;
				this->_zone.selectZone(graphics, value-1);
				this->initialisePlayer(graphics);
				this->_textbox.clearDialogue();
			}
			else if (input.wasKeyPressed(SDL_SCANCODE_D)) {
				this->_isPlayerInLevelSelect = 2;
				this->_textbox.clearDialogue();
			}			
			return true;
		}
		else if (this->_isPlayerInLevelSelect == 2) {
			
		}
	}
	else {
		this->_isPlayerInLevelSelect = 0;
	}


	// movement and switching dimension
	if (_canPlayerMove && _canPlayerSwitchStage && this->_player.getVisible()) {

		if (input.isKeyHeld(SDL_SCANCODE_LEFT) == true) {
			this->_player.moveLeft(this->_canPlayerMove, this->_overworld.getStage(), empty, this->_ticket);
		}
		else if (input.isKeyHeld(SDL_SCANCODE_RIGHT) == true) {
			this->_player.moveRight(this->_canPlayerMove, this->_overworld.getStage(), empty, this->_ticket);
		}
		else if (input.isKeyHeld(SDL_SCANCODE_UP) == true) {
			this->_player.moveUp(this->_canPlayerMove, this->_overworld.getStage(), empty, this->_ticket);
		}
		else if (input.isKeyHeld(SDL_SCANCODE_DOWN) == true) {
			this->_player.moveDown(this->_canPlayerMove, this->_overworld.getStage(), empty, this->_ticket);
		}

		if (!input.isKeyHeld(SDL_SCANCODE_LEFT) && !input.isKeyHeld(SDL_SCANCODE_RIGHT)
			&& !input.isKeyHeld(SDL_SCANCODE_UP) && !input.isKeyHeld(SDL_SCANCODE_DOWN)) {
			this->_player.stopMoving();
		}
	}
	
	const int CURRENT_TIME_MS = SDL_GetTicks64();
	int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
	this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
	LAST_UPDATE_TIME = CURRENT_TIME_MS;

	this->draw(graphics);

	return true;
}

bool Game::openingscreen(Graphics& graphics, Input& input, int& LAST_UPDATE_TIME)
{
	if (input.wasKeyPressed(SDL_SCANCODE_A)) {
		this->_currScreen = OVERWORLD;
		return true;
	}
	else if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE))  {
		return false;
	}

	const int CURRENT_TIME_MS = SDL_GetTicks64();
	int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
	this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
	LAST_UPDATE_TIME = CURRENT_TIME_MS;

	this->draw(graphics);

	return true;
}

bool Game::victoryscreen(Graphics& graphics, Input& input, int& LAST_UPDATE_TIME)
{
	if (input.wasKeyPressed(SDL_SCANCODE_A)) {
		this->_currScreen = OVERWORLD;
		return true;
	}
	else if (input.wasKeyPressed(SDL_SCANCODE_ESCAPE)) {
		return false;
	}

	const int CURRENT_TIME_MS = SDL_GetTicks64();
	int ELAPSED_TIME_MS = CURRENT_TIME_MS - LAST_UPDATE_TIME;
	this->update(std::min(ELAPSED_TIME_MS, MAX_FRAME_TIME), graphics);
	LAST_UPDATE_TIME = CURRENT_TIME_MS;

	this->draw(graphics);

	return true;
}

void Game::initialisePlayer(Graphics& graphics)
{
	this->_player = Player(graphics, this->_zone.getSpawnPoint());
}
