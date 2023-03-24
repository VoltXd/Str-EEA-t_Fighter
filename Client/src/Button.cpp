#include "Button.hpp"

#include <SDL_image.h>
#include <iostream>

#include "Toolbox.hpp"

Button::Button()
{
    m_buttonTexture = nullptr;
    m_isSelected = false;
    m_srcRect = { 0, 0, 0, 0};
    m_dstRect = { 0, 0, 0, 0};
    m_width = 0;
    m_height = 0;
}

Button::~Button()
{
    SDL_DestroyTexture(m_buttonTexture);
}

int Button::initialize(SDL_Renderer* renderer, const char* fileName, const char* text, int x, int y, int w, int h)
{
    m_buttonTexture = IMG_LoadTexture(renderer, fileName);
    int errorCode = SDL_QueryTexture(m_buttonTexture, nullptr, nullptr, &m_srcRect.w, &m_srcRect.h);
    if (errorCode != 0)
    {
        SDL_ShowError("Button Init. error", __FILE__, __LINE__);
        return EXIT_FAILURE;
    }

    m_dstRect = { x - (w / 2), y - (h / 2), w, h };
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;

    return 0;
}


void Button::update(SDL_Point mousePosition)
{
    const float SCALE_FACTOR = 1.2;
    SDL_Rect mousePositionRect = { mousePosition.x, mousePosition.y, 1, 1 };
    if (SDL_HasIntersection(&m_dstRect, &mousePositionRect))
    {
        if (m_isSelected == false)
        {
            m_dstRect.w = m_width * SCALE_FACTOR;
            m_dstRect.h = m_height * SCALE_FACTOR;
            m_dstRect.x = m_x - (m_dstRect.w / 2);
            m_dstRect.y = m_y - (m_dstRect.h / 2);
        }
        m_isSelected = true;
    }
    else
    {
        if (m_isSelected == true)
        {
            m_dstRect.w = m_width;
            m_dstRect.h = m_height;
            m_dstRect.x = m_x - (m_dstRect.w / 2);
            m_dstRect.y = m_y - (m_dstRect.h / 2);
        }
        m_isSelected = false;
    }
}

void Button::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_buttonTexture, &m_srcRect, &m_dstRect);
}