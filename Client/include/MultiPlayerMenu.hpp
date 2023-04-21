#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>
#include "Scene.hpp"
#include "IpPortBox.hpp"
#include "Button.hpp"

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

    void connect();
    void cancel();

    IpPortBox m_ipPortBox;
    Button m_connectButton;
    Button m_cancelButton;

    SDL_Texture* m_buttonTexture;
    TTF_Font* m_textFont;
    SDL_Point m_mousePosition;

    char m_inputString[32];

    bool m_isClicking;
    bool m_isErasing;
};