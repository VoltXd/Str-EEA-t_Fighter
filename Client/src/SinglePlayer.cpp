#include "SinglePlayer.hpp"

#include <iostream>

SinglePlayer::SinglePlayer(SDL_Renderer* renderer)
{
    m_renderer = renderer;

    m_isRunning = true;
    m_nextScene = SceneId::MainMenu;
}

SceneId SinglePlayer::run()
{
    if (initialise() != EXIT_SUCCESS)
        return SceneId::MainMenu;
    
    while (m_isRunning)
    {
        input();
        update();
        render();
    }

    return m_nextScene;
}

int SinglePlayer::initialise()
{

    return EXIT_SUCCESS;
}

void SinglePlayer::input()
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

            case SDL_KEYDOWN:
                if (event.key.keysym.scancode == SDL_SCANCODE_ESCAPE)
                {
                    m_isRunning = false;
                    m_nextScene = SceneId::MainMenu;
                }
                break;
			
			default:
				break;
		}
	}
}

void SinglePlayer::update()
{

}

void SinglePlayer::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_RenderPresent(m_renderer);
}