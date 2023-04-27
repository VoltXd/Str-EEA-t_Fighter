#include "MultiPlayer.hpp"

#include <SDL_image.h>
#include <iostream>

#include "Client_Streeeat_Fighter.hpp"
#include "Toolbox.hpp"


MultiPlayer::MultiPlayer(SDL_Renderer* renderer)
{
    m_renderer = renderer;

    m_playerHeadTexture = nullptr;
    m_playerHandTexture = nullptr;

    m_isRunning = true;
    m_nextScene = SceneId::MainMenu;
}

MultiPlayer::~MultiPlayer()
{
    SDL_DestroyTexture(m_playerHeadTexture);
    SDL_DestroyTexture(m_playerHandTexture);
    closeClientSocket();
}

SceneId MultiPlayer::run()
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

int MultiPlayer::initialise()
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

    connectToSFServer();
    startClientCommunication();

    return EXIT_SUCCESS;
}

void MultiPlayer::input()
{
    // Should be managed by the Send thread
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        switch (event.type)
        {
            case SDL_KEYDOWN:
            if (event.key.keysym.sym == SDLK_ESCAPE)
            {
                m_isRunning = false;
                m_nextScene = SceneId::MainMenu;
            }
            break;
        }
    }
}

void MultiPlayer::update()
{
    // Should be managed by the Recv thread
    updateClientData();
}

void MultiPlayer::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    renderClient(m_renderer);

    SDL_RenderPresent(m_renderer);
}