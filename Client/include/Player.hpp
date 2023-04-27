#pragma once

#include <SDL.h>
#include <chrono>

#include "Boxer.hpp"
class Player : public Boxer
{
    public:
    Player();

    void initialise(SDL_Texture* headTexture, SDL_Texture* handTexture) override;

    void render(SDL_Renderer* renderer) override;
    
    private:
    SDL_Rect m_healthBar;
    SDL_Rect m_healthBarBorder;
    SDL_Rect m_staminaBar;
    SDL_Rect m_staminaBarBorder;
};