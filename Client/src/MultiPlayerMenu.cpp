#include "MultiPlayerMenu.hpp"

#include <iostream>
#include <SDL_image.h>

#include "Settings.hpp"
#include "Toolbox.hpp"

MultiPlayerMenu::MultiPlayerMenu(SDL_Renderer* renderer) 
    : m_ipPortBox(),
      m_inputString(),
      m_connectButton(),
      m_cancelButton()
{
    m_renderer = renderer;
    m_nextScene = SceneId::MainMenu;

    m_textFont = nullptr;

    m_mousePosition = { 0, 0 };
    m_isClicking = false;
    m_isErasing = false;

    m_isRunning = true;
}

MultiPlayerMenu::~MultiPlayerMenu()
{
    SDL_DestroyTexture(m_buttonTexture);
    TTF_CloseFont(m_textFont);
    SDL_StopTextInput();
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
    m_textFont = TTF_OpenFont("imports/fonts/Platinum_Sign.ttf", 48);
    if (m_textFont == nullptr)
    {
        SDL_ShowError("MultiPlayerMenu IP Port Box font error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }
    
    // Load button components
    m_buttonTexture = IMG_LoadTexture(m_renderer, "imports/images/button_blue.png");
    if (m_buttonTexture == nullptr)
    {
        SDL_ShowError("MultiPlayerMenu Load button texture error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    // Init IP/PORT box
    int errorCode = m_ipPortBox.initialise(m_renderer, m_textFont, settings.screenWidth / 2, settings.screenHeight / 2, 0.6 * settings.screenWidth, settings.screenHeight / 4);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MultiPlayerMenu Init. IP/PORT box error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    errorCode = m_connectButton.initialise(m_renderer, m_buttonTexture, m_textFont, "CONNEXION", 3 * settings.screenWidth / 4, 7 * settings.screenHeight / 8, settings.screenWidth / 3, settings.screenHeight / 8);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MultiPlayerMenu Init. button connect error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    errorCode = m_cancelButton.initialise(m_renderer, m_buttonTexture, m_textFont, "ANNULER", settings.screenWidth / 4, 7 * settings.screenHeight / 8, settings.screenWidth / 3, settings.screenHeight / 8);
    if (errorCode != EXIT_SUCCESS)
    {
        SDL_ShowError("MultiPlayerMenu Init. button cancel error", __FILE__, __LINE__);
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
                else if (event.key.keysym.sym == SDLK_BACKSPACE)
                {
                    m_isErasing = true;
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
        m_ipPortBox.updateText(m_renderer, m_inputString, m_isErasing);

    if (m_connectButton.update(m_mousePosition, m_isClicking))
        connect();
    
    if (m_cancelButton.update(m_mousePosition, m_isClicking))
        cancel();

    m_inputString[0] = '\0';
    m_isErasing = false;
    m_isClicking = false;
}

void MultiPlayerMenu::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    m_ipPortBox.render(m_renderer);
    m_connectButton.render(m_renderer);
    m_cancelButton.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void MultiPlayerMenu::connect()
{
    const std::string& ip = m_ipPortBox.getIp();
    int port = m_ipPortBox.getPort();

    std::cout << ip << port;
}

void MultiPlayerMenu::cancel()
{
    m_isRunning = false;
    m_nextScene = SceneId::MainMenu;
}