#ifndef AUDIO_H
#define AUDIO_H

#include "SDL_mixer.h"

#include <iostream>
#include <unordered_map>
#include <memory>
#include <exception>
#include <stdexcept>
#include <string>

class Audio {
public:
	void static setCurrentMusic(std::string key);
	void static toggle();
	void static initializeData();
	void static destroyData();
	
private:
	static std::string _key;
	static std::unordered_map<std::string, Mix_Music*> _music;
	static bool IsMuted;
};


class Foley {
public:
	void static playSound(std::string key);
	void static toggle();
	void static initializeData();
	void static destroyData();
	
private:
	static std::unordered_map<std::string, Mix_Chunk*> _sound;
	static bool IsMuted;
};
#endif