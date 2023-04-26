#pragma once

/* --- Class Player ---
* les attributs en float correspondent � des pourcentages
*/

#include <iostream>
#include <string>

#include <WinSock2.h> 

#include "Data_structs.hpp"
#include "Vec2D.hpp"
#include "Timer.hpp"

#define LOWER_BOUND(value, limit) (value < limit) ? value = limit : value = value 
#define UPPER_BOUND(value, limit) (value > limit) ? value = limit : value = value

class PlayerOnline {
private:
	std::string name;
	ServerToClient_Data lastReceivedData; // derni�res donn�es correctes re�ues
	ServerToClient_Data prevReceivedData; // avant-derni�res donn�es correctes re�ues
	Timer<std::chrono::milliseconds> comTimer; // timer pour la gestion de la communication par rapport au temps
	Timer<std::chrono::microseconds> autoShiftingTimer; // timer pour la gestion du d�placement automatique

	Vec2D leftHandPos, rightHandPos;
	Vec2D headPos;
	char paused;
	float lifeBar;
	float staminaBar;

	// vitesses pour le d�placement automatique
	Vec2D leftHandVel, rightHandVel;
	Vec2D headVel;

public:
	PlayerOnline(std::string playerName);

	// set/get methods
	inline void setLastReceivedData(ServerToClient_Data lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	inline void setPrevReceivedData(ServerToClient_Data prevReceivedData) { this->prevReceivedData = prevReceivedData; };
	inline ServerToClient_Data& getLastReceivedData() { return lastReceivedData; };
	inline ServerToClient_Data& getPrevReceivedData() { return prevReceivedData; };
	inline std::string getName() { return name; };
	inline Vec2D& getLeftHandPos() { return leftHandPos; };
	inline Vec2D& getRightHandPos() { return rightHandPos; };
	inline Vec2D& getHeadPos() { return headPos; };
	inline char isPaused() { return paused; };
	inline float getLifeBar() { return lifeBar; };
	inline float getStaminaBar() { return staminaBar; };

	inline void dataAreReceived() { comTimer.start(); }; // remise � z�ro du timer
	inline unsigned long long checkTime() { return comTimer.getTime(); };

	void pullLastReceivedData(); // stocke les donn�es du dernier datagramme re�u dans les caract�ristiques du joueur
	void setAutoShiftingParameters(); // met � jour les param�tres du d�placement automatique
	void updatePosAutoShifting(float headWidth, float headHeight, float handWidth, float handHeight);
};
