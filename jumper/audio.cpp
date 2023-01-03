#include "audio.h"


Audio::Audio() {
	_music.insert({ "opening", "audio/opening-music.wav" });
	_music.insert({ "game", "audio/game-music.wav" });
	_music.insert({ "victory", "audio/victory-music.wav" });
}

Audio::~Audio() {
	
}

void Audio::setCurrentMusic(std::string key) {
	if (key == this->_currMusicKey) return;

	if (this->_currMusic != nullptr) {
		Mix_HaltMusic();
		Mix_FreeMusic(this->_currMusic);
	}

	this->_currMusicKey = key;
	this->_currMusic = Mix_LoadMUS(this->_music.at(_currMusicKey).c_str());

	if (!IsMuted) {
		this->toggle();
	}
}

void Audio::toggle() {
	if (Mix_PlayingMusic() == 0) {
		//Play the music
		Mix_PlayMusic(_currMusic, -1);
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

bool Audio::IsMuted = false;

Foley::Foley() {
	_sound.insert({ "walk", "audio/walk-sound.wav" });
	_sound.insert({ "collide", "audio/collide-sound.wav" });
	_sound.insert({ "kill", "audio/kill-sound.wav" });
	_sound.insert({ "menu", "audio/menu-sound.wav" });
	_sound.insert({ "transition", "audio/transition-sound.wav" });
	_sound.insert({ "victory", "audio/victory-sound.wav" });
	_sound.insert({ "menu-close", "audio/menu-close-sound.wav" });
	_sound.insert({ "undo", "audio/undo-sound.wav" });
}

void Foley::playSound(std::string key) {
	if (IsMuted) return;
	
	if (key == this->_currChunkKey) {
		Mix_PlayChannel(-1, this->_currChunk, 0);
		return;
	}

	if (this->_currChunk != nullptr) {
		Mix_FreeChunk(this->_currChunk);
	}

	this->_currChunkKey = key;
	this->_currChunk = Mix_LoadWAV(this->_sound.at(_currChunkKey).c_str());
	Mix_PlayChannel(-1, this->_currChunk, 0);
}

void Foley::toggle() {
	IsMuted = !IsMuted;
}

bool Foley::IsMuted = false;

Foley::~Foley() {};