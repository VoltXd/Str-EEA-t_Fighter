#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
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
    SDL_Point m_mousePosition;

    char m_inputString[32];

    bool m_isClicking;
};