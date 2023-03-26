#include "Button.hpp"

#include <iostream>
#include <SDL_image.h>
#include <SDL_ttf.h>

#include "Toolbox.hpp"

Button::Button()
{
    m_buttonTexture = nullptr;
    m_textTexture = nullptr;

    m_buttonDstRect = { 0, 0, 0, 0 };
    m_textDstRect = { 0, 0, 0, 0 };

    m_x = 0;
    m_y = 0;
    m_width = 0;
    m_height = 0;
    m_textWidth = 0;
    m_textHeight = 0;

    m_isSelected = false;
}

Button::~Button()
{
    SDL_DestroyTexture(m_textTexture);
}

int Button::initialise(SDL_Renderer* renderer, SDL_Texture* buttonTexture, TTF_Font* buttonFont, const char* text, int x, int y, int w, int h)
{
    // Texture store
    m_buttonTexture = buttonTexture;

    // Get text surface
    SDL_Color black = { 0, 0, 0 };
    SDL_Surface* textSurface = TTF_RenderText_Blended(buttonFont, text, black);
    if (textSurface == nullptr)
    {
        SDL_ShowError("Button RenderText error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }
    
    // Converse text surface to texture
    m_textTexture = SDL_CreateTextureFromSurface(renderer, textSurface);
    if (m_textTexture == nullptr)
    {
        SDL_ShowError("Button CreateTextureFromSurface error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    // Free surface
    SDL_FreeSurface(textSurface);
    
    // Calculate button destination rectangle
    m_buttonDstRect = { x - (w / 2), y - (h / 2), w, h };
    
    // Calculate text destination rectangle 
    const int textScale = 4;
    SDL_QueryTexture(m_textTexture, nullptr, nullptr, &m_textWidth, &m_textHeight);
    m_textWidth *= (float)h / textScale / m_textHeight;
    m_textHeight = (float)h / textScale;
    m_textDstRect = { x - (m_textWidth / 2), y - (m_textHeight / 2), m_textWidth, m_textHeight };
    
    // Save button position and size
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;

    return 0;
}


bool Button::update(SDL_Point mousePosition, bool isUserClicking)
{
    const float SCALE_FACTOR = 1.2f;
    SDL_Rect mousePositionRect = { mousePosition.x, mousePosition.y, 1, 1 };

    // Check if the button is selected
    bool isClicked = false;
    if (SDL_HasIntersection(&m_buttonDstRect, &mousePositionRect))
    {
        if (m_isSelected == false)
        {
            // The button is being selected, make it bigger
            m_buttonDstRect.w = m_width * SCALE_FACTOR;
            m_buttonDstRect.h = m_height * SCALE_FACTOR;
            m_buttonDstRect.x = m_x - (m_buttonDstRect.w / 2);
            m_buttonDstRect.y = m_y - (m_buttonDstRect.h / 2);
            
            m_textDstRect.w = m_textWidth * SCALE_FACTOR;
            m_textDstRect.h = m_textHeight * SCALE_FACTOR;
            m_textDstRect.x = m_x - (m_textDstRect.w / 2);
            m_textDstRect.y = m_y - (m_textDstRect.h / 2);
            m_isSelected = true;
        }

        if (isUserClicking)
            isClicked = true;
    }
    else
    {
        if (m_isSelected == true)
        {
            // The button is being unselected, make it normal size
            m_buttonDstRect.w = m_width;
            m_buttonDstRect.h = m_height;
            m_buttonDstRect.x = m_x - (m_buttonDstRect.w / 2);
            m_buttonDstRect.y = m_y - (m_buttonDstRect.h / 2);

            m_textDstRect.w = m_textWidth;
            m_textDstRect.h = m_textHeight;
            m_textDstRect.x = m_x - (m_textDstRect.w / 2);
            m_textDstRect.y = m_y - (m_textDstRect.h / 2);
            m_isSelected = false;
        }
    }

    return isClicked;
}

void Button::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_buttonTexture, nullptr, &m_buttonDstRect);
    SDL_RenderCopy(renderer, m_textTexture, nullptr, &m_textDstRect);
}