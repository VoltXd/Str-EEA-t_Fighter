#pragma once

#include <SDL.h>
#include <iostream>

#include "Player.hpp"

class App	{
private:
	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	SDL_Event event;
	bool m_isRunning;
	int m_screenWidth;
	int m_screenHeight;

public:
	App(int width, int height);
	~App();
	int initialise();
	bool exit();
	void drawRect(float x1, float x2, int handSize, SDL_Color color);

	void renderClear();
	void renderPresent();
};