#pragma once

#include "Boxer.hpp"
#include "Player.hpp"

class Bot : public Boxer
{
    public:
    Bot();

    void initialise(SDL_Texture* headTexture, SDL_Texture* handTexture) override;
    void calculateNextAction(const Player& player);
    
    void render(SDL_Renderer* renderer) override;

    private:
};
