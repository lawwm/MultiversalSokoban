#ifndef AUDIO_H
#define AUDIO_H

#include "SDL_mixer.h"

#include <iostream>
#include <unordered_map>

#include <exception>
#include <stdexcept>

class Audio {
public:
	~Audio() {
	};

	void setCurrentMusic(std::string key) {
		
		if (key == this->_currMusicKey) return;

		if (this->_currMusic != nullptr) {
			Mix_HaltMusic();
			Mix_FreeMusic(this->_currMusic);			
		}

		this->_currMusicKey = key;
		this->_currMusic = Mix_LoadMUS(this->_music.at(_currMusicKey).c_str());
		this->toggle();
	}
	
	std::string what(const std::exception_ptr& eptr = std::current_exception())
	{
		if (!eptr) { throw std::bad_exception(); }

		try { std::rethrow_exception(eptr); }
		catch (const std::exception& e) { return e.what(); }
		catch (const std::string& e) { return e; }
		catch (const char* e) { return e; }
		catch (...) { return "who knows"; }
	}
	
	void toggle() {

		try {
			//If there is no music playing
			if (Mix_PlayingMusic() == 0) {
				//Play the music
				Mix_PlayMusic(_currMusic, -1);
			}
			else {
				//If the music is paused
				if (Mix_PausedMusic() == 1)
				{
					//Resume the music
					Mix_ResumeMusic();
				}
				//If the music is playing
				else
				{
					//Pause the music
					Mix_PauseMusic();
				}
			}			
		}
		catch (...) {
			std::cerr << "Caught exception \"" << what() << "\"\n";
		}

	}

	Audio() {
		_music.insert({ "opening", "audio/opening-music.wav" });
		_music.insert({ "game", "audio/game-music.wav" });
		_music.insert({ "victory", "audio/victory-music.wav" });
	}

private:
	std::unordered_map<std::string, std::string> _music;
	std::string _currMusicKey = "";
	Mix_Music* _currMusic = nullptr;
};


class Foley {
public:
	~Foley() {
	};

	void playSound(std::string key) {
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


	Foley() {

		_sound.insert({ "walk", "audio/walk-sound.wav" });
		_sound.insert({ "collide", "audio/collide-sound.wav" });
		_sound.insert({ "kill", "audio/kill-sound.wav" });
		_sound.insert({ "menu", "audio/menu-sound.wav" });
		_sound.insert({ "transition", "audio/transition-sound.wav" });
		_sound.insert({ "victory", "audio/victory-sound.wav" });
		_sound.insert({ "menu-close", "audio/menu-close-sound.wav" });
		_sound.insert({ "undo", "audio/undo-sound.wav" });
	}

private:
	std::unordered_map<std::string, std::string> _sound;
	std::string _currChunkKey = "";
	Mix_Chunk* _currChunk = nullptr;
};
#endif