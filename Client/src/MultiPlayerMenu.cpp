#include "MultiPlayerMenu.hpp"

#include <iostream>

#include "Settings.hpp"
#include "Toolbox.hpp"

MultiPlayerMenu::MultiPlayerMenu(SDL_Renderer* renderer) : m_ipPortBox(IpPortBox())
{
    m_renderer = renderer;
    m_nextScene = SceneId::MainMenu;
}

SceneId MultiPlayerMenu::run()
{
    if (initialise() != EXIT_SUCCESS)
        return SceneId::Quit;

    while (m_isRunning)
    {
        input();
        update();
        render();
    }

    return m_nextScene;
}

int MultiPlayerMenu::initialise()
{
    // Load button components
    m_ipPortBoxFont = TTF_OpenFont("imports/fonts/Platinum_Sign.ttf", 48);
    if (m_ipPortBoxFont == nullptr)
    {
        SDL_ShowError("MultiPlayerMenu IP Port Box font error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    // Init buttons
    int errorCode = m_ipPortBox.initialise(m_renderer, m_ipPortBoxFont, settings.screenWidth / 2, settings.screenHeight / 2, 0.6 * settings.screenWidth, settings.screenHeight / 4);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MainMenu Init. button SP error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    return EXIT_SUCCESS;
}

void MultiPlayerMenu::input()
{
    SDL_Event event;
	while (SDL_PollEvent(&event))
	{
		switch (event.type)
		{
			case SDL_QUIT:
				m_isRunning = false;
                m_nextScene = SceneId::Quit;
				break;
			
			default:
				break;
		}
	}
}

void MultiPlayerMenu::update()
{
    m_ipPortBox.update();
}

void MultiPlayerMenu::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    m_ipPortBox.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}