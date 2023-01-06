#include "audio.h"


void Audio::initializeData() {
	_music = {
		{ "opening", Mix_LoadMUS("audio/opening-music.wav") },
		{ "game", Mix_LoadMUS("audio/game-music.wav") },
		{ "victory", Mix_LoadMUS("audio/victory-music.wav")},
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
		{ "walk", Mix_LoadWAV("audio/walk-sound.wav") },
		{ "collide", Mix_LoadWAV("audio/collide-sound.wav") },
		{ "kill", Mix_LoadWAV("audio/kill-sound.wav") },
		{ "menu", Mix_LoadWAV("audio/menu-sound.wav") },
		{ "transition", Mix_LoadWAV("audio/transition-sound.wav") },
		{ "victory", Mix_LoadWAV("audio/victory-sound.wav") },
		{ "menu-close", Mix_LoadWAV("audio/menu-close-sound.wav") },
		{ "undo", Mix_LoadWAV("audio/undo-sound.wav") }
	};
}

void Foley::destroyData() {
	for (auto& [key, value] : _sound) {
		Mix_FreeChunk(value);
	}
}

bool Foley::IsMuted = false;
std::unordered_map<std::string, Mix_Chunk*> Foley::_sound = {};

