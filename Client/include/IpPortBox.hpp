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
    bool update(SDL_Point mousePosition, bool isUserClicking);
    void render(SDL_Renderer* renderer);

    void updateText(SDL_Renderer* renderer, const char* appendedString);


    private:
    static enum class BoxType
    {
        IP,
        Port
    };

    void updateTexture(SDL_Renderer* renderer, BoxType boxType);

    TTF_Font* m_textFont;

    SDL_Texture* m_ipTextTexture;
    SDL_Rect m_ipTextRect;
    SDL_Rect m_ipRect;
    
    SDL_Texture* m_portTextTexture;
    SDL_Rect m_portTextRect;
    SDL_Rect m_portRect;

    std::string m_ip;
    int m_port;

    bool m_isIpSelected;
    bool m_isPortSelected;
    bool m_isFirstInput;

    int m_x;
    int m_y;
    int m_width;
    int m_height;

    static constexpr unsigned char IP_MAX_LENGTH = 15;
    static constexpr unsigned long PORT_MAX = 65535;
};