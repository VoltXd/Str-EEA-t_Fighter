#pragma once

#include <SDL.h>
#include <chrono>


enum class BoxerLivingStatus
{
    Alive,
    Dead
};

enum class BoxerActionStatus
{
    PunchingLeft,
    PunchingRight,
    Guarding,
    Idle
};

class Boxer
{
    public:
    // Init. method
    virtual void initialise(SDL_Texture* headTexture, SDL_Texture* handTexture) = 0;

    // Update methods
    BoxerLivingStatus getPunched(float punchPosition);
    bool nextAction(BoxerActionStatus action, double dt);

    inline float getHealthPoints() const { return m_healthPoints; }
    inline float getStamina() const { return m_stamina; }

    inline void setHeadPosition(float xHead) { m_xHead = xHead; setHeadTexturePosition(); }
    inline void setRightHandPosition(float xRightHand) { m_xRightHand = xRightHand; setRightHandTexturePosition(); }
    inline void setLeftHandPosition(float xLeftHand) { m_xLeftHand = xLeftHand; setLeftHandTexturePosition(); }

    inline float getRightHandPosition() const { return m_xRightHand; }
    inline float getLeftHandPosition() const { return m_xLeftHand; }
    inline float getHeadPosition() const { return m_xHead; }

    inline float getPunchPosition() const { return m_punchPosition; }

    // Render method
    virtual void render(SDL_Renderer* renderer) = 0;

    private:
    void setHeadTexturePosition();
    void setRightHandTexturePosition();
    void setLeftHandTexturePosition();

    protected:
    SDL_Texture* m_headTexture;
    SDL_Texture* m_handTexture;
    SDL_Rect m_headRect;
    SDL_Rect m_rightHandRect;
    SDL_Rect m_leftHandRect;

    SDL_Rect m_punchRect;

    float m_healthPoints;
    float m_stamina;
    bool m_isPunching;

    float m_xHead;
    float m_xRightHand;
    float m_xLeftHand;

    BoxerActionStatus m_currentActionStatus;

    float m_punchPosition;
    std::chrono::high_resolution_clock::time_point m_punchTimePoint;
    int m_elapsedTimeToPunch;

    static constexpr int START_HP = 100;
    static constexpr int STAMINA_MAX = 100;
    static constexpr int PUNCH_DELAY_MS = 500; 
    static constexpr int PUNCH_WIDTH = 10;
    static constexpr float HEALTH_REGENERATION_PER_SECOND = 1;
    static constexpr float STAMINA_REGENERATION_PER_SECOND = 3;

    static constexpr int STAMINA_COST_PUNCH = 10;
    static constexpr int STAMINA_COST_PUNCHED_WHILE_GUARDING = 20;
    static constexpr int STAMINA_COST_GUARD_PER_SECOND = 5;
};