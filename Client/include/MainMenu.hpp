#pragma once

#include "Scene.hpp"
#include "Button.hpp"

#include <SDL_ttf.h>


class MainMenu : public Scene
{
    public:
    MainMenu(SDL_Renderer* renderer);
    ~MainMenu();

    SceneId run() override;

    private:
    int initialise() override;
    
    void input() override;
    void update() override;
    void render() override;

    void playSingle();
    void playMulti();
    void quit();

    SDL_Texture* m_buttonTexture;
    SDL_Texture* m_backgroundTexture;
    TTF_Font* m_buttonFont;
	
    SDL_Point m_mousePosition;
    bool m_isClicking;
    
    Button m_singlePlayerButton;
    Button m_multiPlayerButton;
    Button m_quitButton;
};