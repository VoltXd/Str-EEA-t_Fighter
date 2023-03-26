#include "MainMenu.hpp"

#include <SDL.h>
#include <SDL_image.h>
#include <iostream>

#include "Settings.hpp"
#include "Toolbox.hpp"

MainMenu::MainMenu(SDL_Renderer* renderer)
{
    m_renderer = renderer;
    m_buttonTexture = nullptr;
    m_buttonFont = nullptr;

    m_mousePosition = { 0, 0 };
    m_isClicking = false;
    
    m_nextScene = SceneId::MainMenu;

    m_singlePlayerButton = Button();
    m_multiPlayerButton = Button();
    m_quitButton = Button();

    m_isRunning = true;
}

SceneId MainMenu::run() 
{
    if (initialise() != EXIT_SUCCESS)
        return SceneId::Quit;
    
    while (m_isRunning)
    {
        input();
        update();
        render();
    }

    SDL_DestroyTexture(m_buttonTexture);
    SDL_DestroyTexture(m_backgroundTexture);
    TTF_CloseFont(m_buttonFont);

    return m_nextScene;
}

int MainMenu::initialise()
{
    // Load background 
    m_backgroundTexture = IMG_LoadTexture(m_renderer, "imports/images/mainmenu_background.jpg");
    if (m_backgroundTexture == nullptr)
    {
        SDL_ShowError("MainMenu Load background texture error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    // Load button components
    m_buttonTexture = IMG_LoadTexture(m_renderer, "imports/images/button_blue.png");
    if (m_buttonTexture == nullptr)
    {
        SDL_ShowError("MainMenu Load button texture error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    m_buttonFont = TTF_OpenFont("imports/fonts/Platinum_Sign.ttf", 36);
    if (m_buttonFont == nullptr)
    {
        SDL_ShowError("MainMenu Load button font error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    // Init buttons
    int errorCode = m_singlePlayerButton.initialise(m_renderer, m_buttonTexture, m_buttonFont, "JOUER", settings.screenWidth / 2, settings.screenHeight / 4, 0.6 * settings.screenWidth, settings.screenHeight / 6);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MainMenu Init. button SP error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    errorCode = m_multiPlayerButton.initialise(m_renderer, m_buttonTexture, m_buttonFont, "MULTIJOUEUR", settings.screenWidth / 2, settings.screenHeight / 2, 0.6 * settings.screenWidth, settings.screenHeight / 6);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MainMenu Init. button MP error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }
    
    errorCode = m_quitButton.initialise(m_renderer, m_buttonTexture, m_buttonFont, "QUITTER", settings.screenWidth / 2, settings.screenHeight * 3 / 4, 0.6 * settings.screenWidth, settings.screenHeight / 6);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MainMenu Init. button Quit error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }


    return EXIT_SUCCESS;
}

void MainMenu::input()
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
			
			case SDL_MOUSEMOTION:
				SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
				break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    m_isClicking = true;
                break;

			default:
				break;
		}
	}
}

void MainMenu::update()
{
    if (m_singlePlayerButton.update(m_mousePosition, m_isClicking))
        playSingle();

    if (m_multiPlayerButton.update(m_mousePosition, m_isClicking))
        playMulti();
        
    if (m_quitButton.update(m_mousePosition, m_isClicking))
        quit();

    m_isClicking = false;
}

void MainMenu::render()
{
	SDL_RenderCopy(m_renderer, m_backgroundTexture, nullptr, nullptr);

    m_singlePlayerButton.render(m_renderer);
    m_multiPlayerButton.render(m_renderer);
    m_quitButton.render(m_renderer);

	SDL_RenderPresent(m_renderer);
}

void MainMenu::playSingle()
{
    m_isRunning = false;
    m_nextScene = SceneId::SinglePlayer;
}

void MainMenu::playMulti()
{
    m_isRunning = false;
    m_nextScene = SceneId::MultiPlayer;
}

void MainMenu::quit()
{
    m_isRunning = false;
    m_nextScene = SceneId::Quit;
}