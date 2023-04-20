#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include "Scene.hpp"
#include "IpPortBox.hpp"

class MultiPlayerMenu : public Scene
{
    public:
    MultiPlayerMenu(SDL_Renderer* renderer);

    SceneId run() override;

    private:
    int initialise() override;

    void input() override;
    void update() override;
    void render() override;

    IpPortBox m_ipPortBox;

    TTF_Font* m_ipPortBoxFont;
};