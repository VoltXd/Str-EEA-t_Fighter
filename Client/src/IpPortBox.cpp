#include "IpPortBox.hpp"

#include "Toolbox.hpp"

IpPortBox::IpPortBox() 
    : m_ip(std::string("127.0.0.1")),
      m_port(50000)
{
    m_textFont = nullptr;

    m_ipTextTexture = nullptr;
    m_ipTextRect = { 0, 0, 0, 0};
    m_ipRect = { 0, 0, 0, 0};

    m_portTextTexture = nullptr;
    m_portTextRect = { 0, 0, 0, 0};
    m_portRect = { 0, 0, 0, 0};

    m_isSelected = false;
    m_isFirstInput = true;
}

IpPortBox::~IpPortBox()
{
    SDL_DestroyTexture(m_ipTextTexture);
    SDL_DestroyTexture(m_portTextTexture);
}

int IpPortBox::initialise(SDL_Renderer* renderer, TTF_Font* textFont, int x, int y, int w, int h)
{
    m_textFont = textFont;

    // Save Box position and size
    m_x = x;
    m_y = y;
    m_width = w;
    m_height = h;

    // Calculate button destination rectangle
    m_ipRect = { x - (w / 2), y - (h / 2), w, h / 2 };
    m_portRect = { x - (w / 2), y, w, h / 2 };

    // Get text surface
    SDL_Color white = { 255, 255, 255 };
    SDL_Surface* ipSurface = TTF_RenderText_Blended(m_textFont, m_ip.c_str(), white);
    if (ipSurface == nullptr)
    {
        SDL_ShowError("IpBox RenderText error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }
    
    // Converse text surface to texture
    m_ipTextTexture = SDL_CreateTextureFromSurface(renderer, ipSurface);
    if (m_ipTextTexture == nullptr)
    {
        SDL_ShowError("IpBox CreateTextureFromSurface error", __FILE__, __LINE__);
        return EXIT_FAILURE;    
    }

    // Free surface
    SDL_FreeSurface(ipSurface);
    
    // Calculate text destination rectangle 
    const int textScale = 3;
    int ipWidth, ipHeight;
    SDL_QueryTexture(m_ipTextTexture, nullptr, nullptr, &ipWidth, &ipHeight);
    ipWidth *= (float)h / 2 / textScale / ipHeight;
    ipHeight = (float)h / 2 / textScale;
    m_ipTextRect = { x - (ipWidth / 2), y - (ipHeight / 2) - (h / 4), ipWidth, ipHeight};

    return EXIT_SUCCESS;
}

void IpPortBox::update()
{

}

void IpPortBox::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderFillRect(renderer, &m_ipRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &m_ipRect);
    SDL_RenderCopy(renderer, m_ipTextTexture, nullptr, &m_ipTextRect);
}