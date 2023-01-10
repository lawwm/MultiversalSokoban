#include "windowsize.h"

int Window::idx = 0;
std::vector<std::tuple<float, float, int, int, int, Uint32>> Window::_windowSettings = { // sprite scale, player speed, screen width, screen height, text size, window size
	{2.0f, 0.15f, 600, 480, 25, 0},
	{3.0f, 0.225f, 900, 720, 38, 0},
	{4.0, 0.30f, 1200, 960, 50, 0}
};



float Window::getSpriteScale() {
	return std::get<0>(_windowSettings[idx]);
}

float Window::getPlayerSpeed() {
	return std::get<1>(_windowSettings[idx]);
}

int Window::getScreenWidth() {
	return std::get<2>(_windowSettings[idx]);
}

int Window::getScreenHeight() {
	return std::get<3>(_windowSettings[idx]);
}

int Window::getTextSize() {
	return std::get<4>(_windowSettings[idx]);
}

Uint32 Window::getScreenSetting() {
	return std::get<5>(_windowSettings[idx]);
}

void Window::setWindowSize(int index) {
	idx = index;
}

