#pragma once

#include <SDL.h>

class Button
{
    public:
    Button();
    ~Button();

    int initialize(SDL_Renderer* renderer, const char* fileName, const char* text, int x, int y, int w, int h);
    void update(SDL_Point mousePosition);
    void render(SDL_Renderer* renderer);

    private:
    SDL_Texture* m_buttonTexture;
    SDL_Rect m_srcRect;
    SDL_Rect m_dstRect;
    int m_x;
    int m_y;
    int m_width;
    int m_height;
    bool m_isSelected;


};