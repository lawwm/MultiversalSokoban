#ifndef AUDIO_H
#define AUDIO_H

#include "SDL_mixer.h"

#include <iostream>
#include <unordered_map>

#include <exception>
#include <stdexcept>

class Audio {
public:
	~Audio();

	void setCurrentMusic(std::string key);
	
	void toggle();

	Audio();

private:
	std::unordered_map<std::string, std::string> _music;
	std::string _currMusicKey = "";
	Mix_Music* _currMusic = nullptr;
	static bool IsMuted;
};


class Foley {
public:
	~Foley();

	void playSound(std::string key);

	void toggle();

	Foley();

private:
	std::unordered_map<std::string, std::string> _sound;
	std::string _currChunkKey = "";
	Mix_Chunk* _currChunk = nullptr;
	static bool IsMuted;
};
#endif