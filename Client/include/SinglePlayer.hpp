#pragma once 

#include <chrono>

#include "Scene.hpp"
#include "Player.hpp"
#include "Bot.hpp"
#include "WebcamManager.hpp"

class SinglePlayer : public Scene
{
    public:
    SinglePlayer(SDL_Renderer* renderer);
    ~SinglePlayer();

    SceneId run() override;

    private:
    int initialise() override;

    void input() override;
    void update() override;
    void render() override;
    
    void checkPunch(Boxer& attacker, Boxer& target);

    SDL_Texture* m_backgroundTexture;
    SDL_Texture* m_playerHeadTexture;
    SDL_Texture* m_playerHandTexture;

    std::chrono::high_resolution_clock::time_point m_previousTimePoint;
    
    Player m_player;
    Bot m_bot;
    WebcamManager m_webcamManager;

    bool m_isCameraInfoAvailable;
};