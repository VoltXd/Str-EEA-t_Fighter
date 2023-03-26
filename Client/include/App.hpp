#pragma once

#include <SDL.h>
#include "Scene.hpp"

class App
{
public:
	App();

	int run();

private:
	int initialise();

	SDL_Window* m_window;
	SDL_Renderer* m_renderer;
	bool m_isRunning;

	SceneId currentScene;
};