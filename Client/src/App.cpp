#include "App.hpp"

#include <iostream>
#include <SDL.h>
#include <SDL_ttf.h>

#include "Toolbox.hpp"
#include "MainMenu.hpp"
#include "SinglePlayer.hpp"
#include "MultiPlayerMenu.hpp"
#include "MultiPlayer.hpp"
#include "Settings.hpp"

App::App()
{
	m_window = nullptr;
	m_renderer = nullptr;
	m_isRunning = true;

	currentScene = SceneId::MainMenu;
	
	settings.screenWidth = 1280;
	settings.screenHeight = 720;
	settings.isFullscreenOn = false;
}

int App::run()
{
	// App initialisation
	if (initialise() != EXIT_SUCCESS)
		return EXIT_FAILURE;

	while (m_isRunning)
	{
		switch (currentScene)
		{
			case SceneId::MainMenu:
			{
				MainMenu mainMenu(m_renderer);
				currentScene = mainMenu.run();
				break;
			}

			case SceneId::SinglePlayer:
			{
				SinglePlayer singlePlayer(m_renderer);
				currentScene = singlePlayer.run();
				break;
			}

			case SceneId::MultiPlayerMenu:
			{
				MultiPlayerMenu multiPlayerMenu(m_renderer);
				currentScene = multiPlayerMenu.run();
				break;
			}

			case SceneId::MultiPlayer:
			{
				MultiPlayer multiPlayer(m_renderer);
				currentScene = multiPlayer.run();
				break;
			}
			
			case SceneId::Quit:
				m_isRunning = false;
				break;
			
			default:
				m_isRunning = false;
				break;
		}
	} 

	// Exit program
	// Kill SDL componants
	TTF_Quit();
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();

	return EXIT_SUCCESS;
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
	m_window = SDL_CreateWindow("Str[EEA]t Fighter", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, settings.screenWidth, settings.screenHeight, SDL_WINDOW_SHOWN);
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

	// Enable SDL_TTF
	errorCode = TTF_Init();
	if (errorCode != 0)
	{
		SDL_ShowError("SDL TTF_Init error", __FILE__, __LINE__);
		return EXIT_FAILURE;
	}

	return EXIT_SUCCESS;
}
