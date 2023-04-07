#include "Boxer.hpp"

#include "Settings.hpp"

BoxerLivingStatus Boxer::takeDamage(float rawDamage)
{
    return BoxerLivingStatus::Alive;
}

bool Boxer::nextAction(BoxerActionStatus action)
{
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
