#pragma once 

#include "Scene.hpp"

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
};