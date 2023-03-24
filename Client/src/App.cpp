#include "App.hpp"

#include <SDL.h>
#include <iostream>

#include "Toolbox.hpp"

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

	testButton.initialize(m_renderer, "imports/images/button_blue.png", "Test", m_screenWidth / 2, m_screenHeight / 2, m_screenWidth / 6, m_screenHeight / 6);

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
		SDL_ShowError("SDL Init. error", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// Create window
	m_window = SDL_CreateWindow("Str[EEA]t Fighter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, m_screenWidth, m_screenHeight, SDL_WINDOW_SHOWN);
	if (m_window == nullptr)
	{
		SDL_ShowError("SDL Window creation error", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// Create renderer
	m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED);
	if (m_renderer == nullptr)
	{
		SDL_ShowError("SDL Renderer creation error", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	// Enable color blending
	errorCode = SDL_SetRenderDrawBlendMode(m_renderer, SDL_BLENDMODE_BLEND);
	if (errorCode != 0)
	{
		SDL_ShowError("SDL color blend mode error", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	return 0;
}

void App::input()
{
	SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				m_isRunning = false;
				break;
			
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
				break;

			default:
				break;
		}

	}
}

void App::update()
{
	testButton.update(m_mousePosition);
}

void App::render()
{
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 128);
	SDL_RenderClear(m_renderer);

	testButton.render(m_renderer);

	SDL_RenderPresent(m_renderer);
}
