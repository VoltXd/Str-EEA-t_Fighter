#pragma once 

#include "Scene.hpp"
#include "Player.hpp"
#include "Bot.hpp"

class SinglePlayer : public Scene
{
    public:
    SinglePlayer(SDL_Renderer* renderer);

    SceneId run() override;

    private:
    int initialise() override;

    void input() override;
    void update() override;
    void render() override;
    
    SDL_Texture* m_backgroundTexture;
    SDL_Texture* m_playerHeadTexture;
    SDL_Texture* m_playerHandTexture;
    
    Player m_player;
    Bot m_bot;
};