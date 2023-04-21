#include "MultiPlayerMenu.hpp"

#include <iostream>

#include "Settings.hpp"
#include "Toolbox.hpp"

MultiPlayerMenu::MultiPlayerMenu(SDL_Renderer* renderer) 
    : m_ipPortBox(),
      m_inputString() 
{
    m_renderer = renderer;
    m_nextScene = SceneId::MainMenu;

    m_ipPortBoxFont = nullptr;

    m_mousePosition = { 0, 0 };
    m_isClicking = false;

    m_isRunning = true;
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

    TTF_CloseFont(m_ipPortBoxFont);
    SDL_StopTextInput();
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

            case SDL_KEYDOWN:
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_isRunning = false;
                    m_nextScene = SceneId::MainMenu;
                }
                break;

            case SDL_MOUSEMOTION:
                SDL_GetMouseState(&m_mousePosition.x, &m_mousePosition.y);
                break;

            case SDL_MOUSEBUTTONDOWN:
                if (event.button.button == SDL_BUTTON_LEFT)
                    m_isClicking = true;
                break;

            case SDL_TEXTINPUT:
                strncpy(m_inputString, event.text.text, 31);
                break;
                
			default:
				break;
		}
	}
}

void MultiPlayerMenu::update()
{
    if (m_ipPortBox.update(m_mousePosition, m_isClicking))
        m_ipPortBox.updateText(m_renderer, m_inputString);

    m_inputString[0] = '\0';
    m_isClicking = false;
}

void MultiPlayerMenu::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    m_ipPortBox.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}