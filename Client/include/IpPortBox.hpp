#pragma once

#include <SDL.h>
#include <SDL_ttf.h>
#include <string>

class IpPortBox
{
    public:
    IpPortBox();
    ~IpPortBox();

    int initialise(SDL_Renderer* renderer, TTF_Font* textFont, int x, int y, int w, int h);
    void update();
    void render(SDL_Renderer* renderer);

    private:
    void updateText();

    TTF_Font* m_textFont;

    SDL_Texture* m_ipTextTexture;
    SDL_Rect m_ipTextRect;
    SDL_Rect m_ipRect;
    
    SDL_Texture* m_portTextTexture;
    SDL_Rect m_portTextRect;
    SDL_Rect m_portRect;

    std::string m_ip;
    int m_port;

    bool m_isSelected;
    bool m_isFirstInput;

    int m_x;
    int m_y;
    int m_width;
    int m_height;
};