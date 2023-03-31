#pragma once

#include <SDL.h>
#include <chrono>

enum class PlayerLivingStatus
{
    Alive,
    Dead
};

enum class PlayerActionStatus
{
    PunchingLeft,
    PunchingRight,
    Guarding,
    Idle
};

class Player
{
    public:
    Player();

    // Init. method
    void initialise(SDL_Texture* headTexture, SDL_Texture* handTexture);

    // Update methods
    PlayerLivingStatus takeDamage(float rawDamage);
    bool nextAction(PlayerActionStatus action);

    inline float getHealthPoints() { return m_healthPoints; }
    inline float getStamina() { return m_stamina; }

    inline void setRightHandPosition(float xRightHand) { m_xRightHand = xRightHand; }
    inline void setLeftHandPosition(float xLeftHand) { m_xLeftHand = xLeftHand; }
    inline void setHeadPosition(float xHead) { m_xHead = xHead; }

    inline float getRightHandPosition() { return m_xRightHand; }
    inline float getLeftHandPosition() { return m_xLeftHand; }
    inline float getHeadPosition() { return m_xHead; }

    // Render method
    void render(SDL_Renderer* renderer);

    private:
    SDL_Texture* m_headTexture;
    SDL_Texture* m_handTexture;
    SDL_Rect m_headRect;
    SDL_Rect m_rightHandRect;
    SDL_Rect m_leftHandRect;

    float m_healthPoints;
    float m_stamina;
    bool m_isPunching;

    float m_xHead;
    float m_xRightHand;
    float m_xLeftHand;

    PlayerActionStatus m_currentActionStatus;

    std::chrono::high_resolution_clock::time_point m_punchTimePoint;

    static const int START_HP = 100;
    static const int STAMINA_MAX = 100;
};