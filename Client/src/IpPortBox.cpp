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

    m_isIpSelected = false;
    m_isPortSelected = false;
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
    
    updateTexture(renderer, BoxType::IP);
    updateTexture(renderer, BoxType::Port);

    return EXIT_SUCCESS;
}

/// @brief Updates the IP Port Box Selection state
/// @param mousePosition The current mouse's position. Used to know whether the mouse is on the box (IP or Port) or not.
/// @param isUserClicking Should be true when the user right clicks. Used to select the box.
/// @return Private member m_isSelected
bool IpPortBox::update(SDL_Point mousePosition, bool isUserClicking)
{
    SDL_Rect mousePositionRect = { mousePosition.x, mousePosition.y, 1, 1 };

    // Check if the button is selected
    bool isClicked = false;
    if (isUserClicking)
    {
        if (SDL_HasIntersection(&m_ipRect, &mousePositionRect))
        {
            SDL_StartTextInput();
            m_isIpSelected = true;
            m_isPortSelected = false;
        }
        else if (SDL_HasIntersection(&m_portRect, &mousePositionRect))
        {
            SDL_StartTextInput();
            m_isPortSelected = true;
            m_isIpSelected = false;
        }  
        else
        {
            SDL_StopTextInput();
            m_isIpSelected = false;
            m_isPortSelected = false;
        }
    }

    return m_isIpSelected || m_isPortSelected;
}

void IpPortBox::render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 16, 16, 16, 255);
    SDL_RenderFillRect(renderer, &m_ipRect);
    SDL_RenderFillRect(renderer, &m_portRect);

    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderDrawRect(renderer, &m_ipRect);
    SDL_RenderDrawRect(renderer, &m_portRect);
    SDL_RenderCopy(renderer, m_ipTextTexture, nullptr, &m_ipTextRect);
    SDL_RenderCopy(renderer, m_portTextTexture, nullptr, &m_portTextRect);
}

void IpPortBox::updateText(SDL_Renderer* renderer, const char* appendedString, bool isErasing)
{
    if (m_isFirstInput)
    {
        m_ip.clear();
        m_port = 0;
        m_isFirstInput = false;
        updateTexture(renderer, BoxType::IP);
        updateTexture(renderer, BoxType::Port);
    }

    if (isErasing)
    {
        if (m_isIpSelected)
        {
            m_ip.clear();
            updateTexture(renderer, BoxType::IP);
        }
        else if (m_isPortSelected)
        {
            m_port = 0;
            updateTexture(renderer, BoxType::Port);
        }
    }

    size_t stringLength = strlen(appendedString);
    for (int i = 0; i < stringLength; i++)
    {
        if (m_isIpSelected)
        {
            if (m_ip.length() >= IP_MAX_LENGTH)
                return;

            if (('0' <= appendedString[i] && appendedString[i] <= '9') || appendedString[i] == '.')
            {
                m_ip.push_back(appendedString[i]);
                updateTexture(renderer, BoxType::IP);
            }
        }
        else if (m_isPortSelected)
        {
            int digitToAppend = appendedString[i] - '0';
            if (m_port * 10 + digitToAppend > PORT_MAX)
                return;

            if ('0' <= appendedString[i] && appendedString[i] <= '9')
            {
                m_port *= 10;
                m_port += digitToAppend;
                updateTexture(renderer, BoxType::Port);
            }
        }
    }
}

void IpPortBox::updateTexture(SDL_Renderer* renderer, BoxType boxType)
{
    SDL_Texture* texture;
    SDL_Rect* rect;
    std::string* text;
    int yOffset;  

    if (boxType == BoxType::IP)
    {
        texture = m_ipTextTexture;
        rect = &m_ipTextRect;
        yOffset = -m_height / 4;  
        if (m_ip.empty())
            text = new std::string(" ");
        else
            text = &m_ip;
    }
    else 
    {
        texture = m_portTextTexture;
        rect = &m_portTextRect;
        text = new std::string(std::to_string(m_port));
        yOffset = m_height / 4;
    }

    SDL_DestroyTexture(texture);

    // Get text surface
    constexpr SDL_Color white = { 255, 255, 255 };
    SDL_Surface* surface = TTF_RenderText_Blended(m_textFont, text->c_str(), white);
    if (surface == nullptr)
    {
        SDL_ShowError("IpPortBox RenderText error", __FILE__, __LINE__);
    }
    
    // Converse text surface to texture
    texture = SDL_CreateTextureFromSurface(renderer, surface);
    if (texture == nullptr)
    {
        SDL_ShowError("IpBox CreateTextureFromSurface error", __FILE__, __LINE__);
    }

    // Free surface
    SDL_FreeSurface(surface);
    
    // Calculate text destination rectangle 
    const int textScale = 3;
    int textWidth, textHeight;
    SDL_QueryTexture(texture, nullptr, nullptr, &textWidth, &textHeight);
    textWidth *= (float)m_height / 2 / textScale / textHeight;
    textHeight = (float)m_height / 2 / textScale;


    *rect = { m_x - (textWidth / 2), m_y - (textHeight / 2) + yOffset, textWidth, textHeight};

    if (BoxType::IP == boxType)
    {
        m_ipTextTexture = texture;
        if (m_ip.empty())
            delete text;
    }
    else
    {
        m_portTextTexture = texture;
        delete text;
    }
}