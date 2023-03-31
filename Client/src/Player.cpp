#include "Player.hpp"

#include "Settings.hpp"

Player::Player()
{
    m_headTexture = nullptr;
    m_handTexture = nullptr;
    m_headRect = { 0, 0, 0, 0};
    m_rightHandRect = { 0, 0, 0, 0};
    m_leftHandRect = { 0, 0, 0, 0};
    
    m_healthPoints = START_HP;
    m_stamina = STAMINA_MAX;
    m_isPunching = false;

    m_xHead = 50;
    m_xRightHand = 70;
    m_xLeftHand = 30;

    m_currentActionStatus = PlayerActionStatus::Idle;

    m_punchTimePoint = std::chrono::high_resolution_clock::now();
}

void Player::initialise(SDL_Texture* headTexture, SDL_Texture* handTexture)
{
    // Save texture ptr
    m_headTexture = headTexture;
    m_handTexture = handTexture;

    const int MAX_PIXEL = settings.screenHeight;

    // Set head rectangle
    int h = MAX_PIXEL / 10;
    SDL_QueryTexture(m_headTexture, nullptr, nullptr, &m_headRect.w, &m_headRect.h);
    m_headRect.w *= (float) h / m_headRect.h;
    m_headRect.h = (float) h;
    m_headRect.x = m_xHead * 0.01 * settings.screenWidth - m_headRect.w / 2;
    m_headRect.y = settings.screenHeight * 7 / 8 - m_headRect.h / 2;

    // Set hands rectangle
    h = MAX_PIXEL / 14;
    SDL_QueryTexture(m_handTexture, nullptr, nullptr, &m_rightHandRect.w, &m_rightHandRect.h);
    m_rightHandRect.w *= (float) h / m_rightHandRect.h;
    m_rightHandRect.h = (float) h;
    m_rightHandRect.x = m_xRightHand * 0.01 * settings.screenWidth - m_rightHandRect.w / 2;
    m_rightHandRect.y = settings.screenHeight * 7 / 8 - m_rightHandRect.h / 2;

    m_leftHandRect.w = m_rightHandRect.w;
    m_leftHandRect.h = m_rightHandRect.h;
    m_leftHandRect.x = m_xLeftHand * 0.01 * settings.screenWidth - m_leftHandRect.w / 2;
    m_leftHandRect.y = settings.screenHeight * 7 / 8 - m_leftHandRect.h / 2;
}

PlayerLivingStatus Player::takeDamage(float rawDamage)
{
    return PlayerLivingStatus::Alive;
}

bool Player::nextAction(PlayerActionStatus action)
{
    return false;
}

void Player::render(SDL_Renderer* renderer)
{
    SDL_RenderCopy(renderer, m_headTexture, nullptr, &m_headRect);
    SDL_RenderCopy(renderer, m_handTexture, nullptr, &m_rightHandRect);
    SDL_RenderCopyEx(renderer, m_handTexture, nullptr, &m_leftHandRect, 0, nullptr, SDL_RendererFlip::SDL_FLIP_HORIZONTAL);
}