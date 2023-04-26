#include "Boxer.hpp"

#include "Settings.hpp"
#include "Toolbox.hpp"

BoxerLivingStatus Boxer::getPunched(float punchPosition)
{
    if (punchPosition - PUNCH_WIDTH / 2 <= m_xHead && m_xHead <= punchPosition + PUNCH_WIDTH / 2)
    {
        // TODO: add RNG
        if (m_currentActionStatus == BoxerActionStatus::Guarding)
        {
            m_stamina -= STAMINA_COST_PUNCHED_WHILE_GUARDING;
            m_healthPoints -= 5;
        }
        else 
            m_healthPoints -= 20;
    }

    if (m_healthPoints <= 0)
        return BoxerLivingStatus::Dead;
    else
        return BoxerLivingStatus::Alive;
}

bool Boxer::nextAction(BoxerActionStatus action, double dt)
{
    m_currentActionStatus = action;

    switch (m_currentActionStatus)
    {
        case BoxerActionStatus::Idle:
            m_healthPoints += HEALTH_REGENERATION_PER_SECOND * dt;
            m_healthPoints = limitToInterval(m_healthPoints, 0, START_HP);
            
            m_stamina += STAMINA_REGENERATION_PER_SECOND * dt;
            m_stamina = limitToInterval(m_stamina, -std::numeric_limits<double>::infinity(), STAMINA_MAX);
            break;
        
        case BoxerActionStatus::PunchingRight:
            if (!m_isPunching && m_stamina > STAMINA_COST_PUNCH)
            {
                m_punchTimePoint = std::chrono::high_resolution_clock::now();
                m_punchPosition = m_xRightHand;
                m_punchRect.x = m_punchPosition * settings.screenWidth * 0.01 - (m_punchRect.w / 2);
                m_isPunching = true;
                m_stamina -= STAMINA_COST_PUNCH;
            }
            break;
        
        case BoxerActionStatus::PunchingLeft:
            if (!m_isPunching && m_stamina > STAMINA_COST_PUNCH)
            {
                m_punchTimePoint = std::chrono::high_resolution_clock::now();
                m_punchPosition = m_xLeftHand;
                m_punchRect.x = m_punchPosition * settings.screenWidth * 0.01 - (m_punchRect.w / 2);
                m_isPunching = true;
                m_stamina -= STAMINA_COST_PUNCH;
            }
            break;
        
        case BoxerActionStatus::Guarding:
            m_stamina -= STAMINA_COST_GUARD_PER_SECOND * dt;
            break;

        default:
            break;
    }

    if (m_isPunching)
    {
        std::chrono::high_resolution_clock::time_point now = std::chrono::high_resolution_clock::now();
        m_elapsedTimeToPunch = std::chrono::duration_cast<std::chrono::milliseconds>(now - m_punchTimePoint).count();
        // m_remainingTimeToPunch = PUNCH_DELAY_MS - elapsedTime;

        if (m_elapsedTimeToPunch > PUNCH_DELAY_MS)
        {
            m_isPunching = false;
            return true;
        }
    }

    return false;
}

void Boxer::setHeadTexturePosition()
{
    m_headRect.x = m_xHead * 0.01 * settings.screenWidth - m_headRect.w / 2;
}

void Boxer::setRightHandTexturePosition()
{
    m_rightHandRect.x = m_xRightHand * 0.01 * settings.screenWidth - m_rightHandRect.w / 2;
}


void Boxer::setLeftHandTexturePosition()
{
    m_leftHandRect.x = m_xLeftHand * 0.01 * settings.screenWidth - m_leftHandRect.w / 2;
}
