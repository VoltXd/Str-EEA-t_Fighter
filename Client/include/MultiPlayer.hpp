#pragma once

#include <SDL.h>

#include "Scene.hpp"
#include "WebcamManager.hpp"

class MultiPlayer : public Scene
{
    public:
    MultiPlayer(SDL_Renderer* renderer);
    ~MultiPlayer();

    SceneId run() override;
    
    private:
    int initialise() override;

    // Game loop
    void input() override;
    void update() override;
    void render() override;

    SDL_Texture* m_playerHeadTexture;
    SDL_Texture* m_playerHandTexture;
};