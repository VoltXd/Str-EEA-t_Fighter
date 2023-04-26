#include "SinglePlayer.hpp"

#include <iostream>
#include <SDL_image.h>

#include "Settings.hpp"
#include "Toolbox.hpp"

// Dummy controler
static bool isPlayerStartingPunchingLeft = false;
static bool isPlayerStartingPunchingRight = false;

SinglePlayer::SinglePlayer(SDL_Renderer* renderer)
    : m_player(),
      m_bot(),
      m_webcamManager(renderer)
{
    m_renderer = renderer;

    m_isRunning = true;
    m_nextScene = SceneId::MainMenu;

    m_backgroundTexture = nullptr;
    m_playerHeadTexture = nullptr;
    m_playerHandTexture = nullptr;

    m_previousTimePoint = std::chrono::high_resolution_clock::now();

    m_isCameraInfoAvailable = false;
}

SceneId SinglePlayer::run()
{
    if (initialise() != EXIT_SUCCESS)
        return SceneId::MainMenu;

    // Calibrate before entering the game loop
    if (!m_webcamManager.calibrate(m_renderer))
        return SceneId::MainMenu;
    
    while (m_isRunning)
    {
        input();
        update();
        render();
    }

    return m_nextScene;
}

SinglePlayer::~SinglePlayer() 
{
    // Free textures
    SDL_DestroyTexture(m_backgroundTexture);
    SDL_DestroyTexture(m_playerHeadTexture);
    SDL_DestroyTexture(m_playerHandTexture);
}

int SinglePlayer::initialise()
{
    // Load background 
    m_backgroundTexture = IMG_LoadTexture(m_renderer, "imports/images/singleplayer_background.png");
    if (m_backgroundTexture == nullptr)
    {
        SDL_ShowError("SinglePlayer Load background texture error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

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
    m_bot.initialise(m_playerHeadTexture, m_playerHandTexture);

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
                if (event.key.keysym.sym == SDLK_ESCAPE)
                {
                    m_isRunning = false;
                    m_nextScene = SceneId::MainMenu;
                }
                break;
			
			default:
				break;
		}
	}

    // Open cv Head & hands detection Input
    m_isCameraInfoAvailable = m_webcamManager.nextAction();
}

void SinglePlayer::update()
{
    // Player controls
    m_player.setHeadPosition(m_webcamManager.getHeadX());
    m_player.setLeftHandPosition(m_webcamManager.getLeftHandX());
    m_player.setRightHandPosition(m_webcamManager.getRightHandX());

    // Aim bot
    m_bot.calculateNextAction(m_player);

    // Setting next player action
    BoxerActionStatus playerAction;
    if (isPlayerStartingPunchingLeft)
    {
        playerAction = BoxerActionStatus::PunchingLeft;
        isPlayerStartingPunchingLeft = false;
    }
    else if (isPlayerStartingPunchingRight)
    {
        playerAction = BoxerActionStatus::PunchingRight;
        isPlayerStartingPunchingRight = false;
    }
    else 
    {
        playerAction = BoxerActionStatus::Idle;
    }

    // Getting delta time
    std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
    double dt = std::chrono::duration_cast<std::chrono::nanoseconds>(now - m_previousTimePoint).count() * 1e-9;
    
    // Executing player & bot next action
    bool isPlayerPunchingNow = m_player.nextAction(playerAction, dt);
    bool isBotPunchingNow = m_bot.nextAction(BoxerActionStatus::PunchingLeft, dt);
    
    // Checking for player punch on this frame
    if (isPlayerPunchingNow)
        checkPunch(m_player, m_bot);
    
    if (isBotPunchingNow)
        checkPunch(m_bot, m_player);

    m_previousTimePoint = now;
}

void SinglePlayer::render()
{
    SDL_SetRenderDrawColor(m_renderer, 0, 0, 0, 255);
    SDL_RenderClear(m_renderer);

    SDL_RenderCopy(m_renderer, m_backgroundTexture, nullptr, nullptr);

    m_player.render(m_renderer);
    m_bot.render(m_renderer);

    SDL_RenderPresent(m_renderer);
}

void SinglePlayer::checkPunch(Boxer& attacker, Boxer& target)
{
    BoxerLivingStatus targetLivingStatus = target.getPunched(attacker.getPunchPosition());
    if (targetLivingStatus == BoxerLivingStatus::Dead)
    {
        m_isRunning = false;
        m_nextScene = SceneId::MainMenu;
    }   
}