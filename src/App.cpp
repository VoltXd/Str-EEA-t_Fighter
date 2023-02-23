#include <SDL.h>

#include "App.hpp"

App::App()
{
	m_window = nullptr;
	m_renderer = nullptr;
	m_isRunning = true;
	m_screenWidth = 1280;
	m_screenHeight = 720;
}

int App::run()
{
	// App initialisation
	int initErrorCode = initialise();
	if (initErrorCode != 0)
		return initErrorCode;

	while (m_isRunning)
	{
		input();
		update();
		render();
	}

	// Exit program
	// Kill SDL componants
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	return 0;
}

int App::initialise()
{
	int errorCode = 0;
	
	// Init. SDL
	errorCode = SDL_Init(SDL_INIT_VIDEO | SDL_INIT_EVENTS);
	if (errorCode != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Init. error", SDL_GetError(), nullptr);
		return errorCode;
	}

	// Create window
	m_window = SDL_CreateWindow("Str[EEA]t Fighter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Window creation error", SDL_GetError(), nullptr);
		return errorCode;
	}

	// Create renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL Renderer creation error", SDL_GetError(), nullptr);
		return errorCode;
	}

	// Enable color blending
	errorCode = SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	if (errorCode != 0)
	{
		SDL_ShowSimpleMessageBox(SDL_MESSAGEBOX_ERROR, "SDL color blend mode error", SDL_GetError(), nullptr);
		return errorCode;
	}

	return 0;
}

void App::input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			m_isRunning = false;
		}

	}
}

void App::update()
{

}

void App::render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 128);
	SDL_RenderClear(m_renderer);

	SDL_Rect testRect{ m_screenWidth / 2, m_screenHeight / 2, 100, 100 };
	SDL_SetRenderDrawColor(m_renderer, 255, 255, 255, 128);
	SDL_RenderFillRect(m_renderer, &testRect);

	SDL_RenderPresent(m_renderer);
}