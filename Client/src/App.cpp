#include <SDL.h>

#include "App.hpp"

App::App(int width, int height)
{
	m_window = nullptr;
	m_renderer = nullptr;
	m_isRunning = true;
	m_screenWidth = width;
	m_screenHeight = height;

	initialise();
}

App::~App() {
	// Exit program
	// Kill SDL componants
	SDL_DestroyRenderer(m_renderer);
	SDL_DestroyWindow(m_window);
	SDL_Quit();
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

bool App::exit()
{
	while (SDL_PollEvent(&event))
	{
		if (event.type == SDL_QUIT)
		{
			return true;
		}
	}
	return false;
}

void App::drawRect(float x1, float x2, int w, int h, SDL_Color color)
{
	SDL_Rect rect1{ x1*m_screenWidth/100 - w / 2, x2*m_screenHeight/100 - h/2, w, h };
	SDL_SetRenderDrawColor(m_renderer, color.r, color.g , color.b, color.a);
	SDL_RenderFillRect(m_renderer, &rect1);
}

void App::renderClear() {
	SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
	SDL_RenderClear(m_renderer);
}

void App::renderPresent() {
	SDL_RenderPresent(m_renderer);
}