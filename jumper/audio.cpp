#include "audio.h"


void Audio::initializeData() {
	_music = {
		{ "opening", Mix_LoadMUS(globals::opening_music.c_str()) },
		{ "game", Mix_LoadMUS(globals::game_music.c_str()) },
		{ "victory", Mix_LoadMUS(globals::victory_music.c_str())},
	};
}

void Audio::destroyData()
{
	for (auto& [key, value] : _music) {
		Mix_FreeMusic(value);
	}
}

void Audio::setCurrentMusic(std::string key) {
	if (key == _key) return;


	Mix_HaltMusic();
	_key = key;

	if (!IsMuted) {
		Audio::toggle();
	}
}

void Audio::toggle() {
	if (Mix_PlayingMusic() == 0) {
		//Play the music
		Mix_PlayMusic(_music[_key], -1);
		IsMuted = false;
	}
	else {
		//If the music is paused
		if (Mix_PausedMusic() == 1)
		{
			//Resume the music
			Mix_ResumeMusic();
			IsMuted = false;
		}
		//If the music is playing
		else
		{
			//Pause the music
			Mix_PauseMusic();
			IsMuted = true;
		}
	}
}

bool Audio::IsMuted = true;
std::unordered_map<std::string, Mix_Music*> Audio::_music = {};
std::string Audio::_key = "";

void Foley::playSound(std::string key) {
	if (IsMuted || _sound.find(key) == _sound.end()) return;
	
	Mix_PlayChannel(-1, _sound[key], 0);
}

void Foley::toggle() {
	IsMuted = !IsMuted;
}

void Foley::initializeData() {
	_sound = {
		{ "walk", Mix_LoadWAV(globals::walk_sound.c_str()) },
		{ "collide", Mix_LoadWAV(globals::collide_sound.c_str()) },
		{ "kill", Mix_LoadWAV(globals::kill_sound.c_str()) },
		{ "menu", Mix_LoadWAV(globals::menu_sound.c_str()) },
		{ "transition", Mix_LoadWAV(globals::transition_sound.c_str()) },
		{ "victory", Mix_LoadWAV(globals::victory_sound.c_str()) },
		{ "menu-close", Mix_LoadWAV(globals::menu_close_sound.c_str()) },
		{ "undo", Mix_LoadWAV(globals::undo_sound.c_str()) }
	};
}

void Foley::destroyData() {
	for (auto& [key, value] : _sound) {
		Mix_FreeChunk(value);
	}
}

bool Foley::IsMuted = false;
std::unordered_map<std::string, Mix_Chunk*> Foley::_sound = {};

