#pragma once

#include <SDL.h>
#include <SDL_ttf.h>

class Button
{
    public:
    Button();
    ~Button();

    int initialise(SDL_Renderer* renderer, SDL_Texture* buttonTexture, TTF_Font* buttonFont, const char* text, int x, int y, int w, int h);
    bool update(SDL_Point mousePosition, bool isUserClicking);
    void render(SDL_Renderer* renderer);

    private:
    SDL_Texture* m_buttonTexture;
    SDL_Texture* m_textTexture;
    SDL_Rect m_buttonDstRect;
    SDL_Rect m_textDstRect;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    int m_textWidth;
    int m_textHeight;
    bool m_isSelected;
};