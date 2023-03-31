#include "SinglePlayer.hpp"

#include <iostream>
#include <SDL_image.h>

#include "Settings.hpp"
#include "Toolbox.hpp"

// Dummy controler
static float tempXHead = 50;
static bool isLeftMoving = false;
static bool isRightMoving = false;

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

    // Free textures
    SDL_DestroyTexture(m_playerHeadTexture);
    SDL_DestroyTexture(m_playerHandTexture);

    return m_nextScene;
}

int SinglePlayer::initialise()
{
    // Load player Head 
    m_playerHeadTexture = IMG_LoadTexture(m_renderer, "imports/images/cursor.png");
    if (m_playerHeadTexture == nullptr)
    {
        SDL_ShowError("SinglePlayer Load player's head texture error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }
    
    // Load player Head 
    m_playerHandTexture = IMG_LoadTexture(m_renderer, "imports/images/cursor.png");
    if (m_playerHandTexture == nullptr)
    {
        SDL_ShowError("Single Load player's hand texture error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    m_player.initialise(m_playerHeadTexture, m_playerHandTexture);

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

            case SDL_MOUSEMOTION:
                tempXHead = 100.0f * event.motion.x / settings.screenWidth;
                break;
            
            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    isLeftMoving = true;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    isRightMoving = true;
                break;
            
            case SDL_MOUSEBUTTONUP:
                if (event.button.button == SDL_BUTTON_LEFT)
                    isLeftMoving = false;
                else if (event.button.button == SDL_BUTTON_RIGHT)
                    isRightMoving = false;
                break;
			
			default:
				break;
		}
	}
}

void SinglePlayer::update()
{
    m_player.setHeadPosition(tempXHead);
    if (isLeftMoving)
        m_player.setLeftHandPosition(tempXHead);
    if (isRightMoving)
        m_player.setRightHandPosition(tempXHead);
}

void SinglePlayer::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    m_player.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}