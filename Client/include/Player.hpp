#pragma once

/* --- Class Player ---
* les attributs en float correspondent à des pourcentages
*/

#include <iostream>
#include <string>

#include <WinSock2.h> 

#include "Data_structs.hpp"
#include "Vec2D.hpp"
#include "Timer.hpp"

#define LOWER_BOUND(value, limit) (value < limit) ? value = limit : value = value 
#define UPPER_BOUND(value, limit) (value > limit) ? value = limit : value = value

class Player {
private:
	std::string name;
	ServerToClient_Data_TypeDef lastReceivedData; // dernières données correctes reçues
	ServerToClient_Data_TypeDef prevReceivedData; // avant-dernières données correctes reçues
	Timer<std::chrono::milliseconds> comTimer; // timer pour la gestion de la communication par rapport au temps
	Timer<std::chrono::microseconds> autoShiftingTimer; // timer pour la gestion du déplacement automatique

	Vec2D leftHandPos, rightHandPos;
	Vec2D headPos;
	char paused;
	float lifeBar;
	float staminaBar;

	// vitesses pour le déplacement automatique
	Vec2D leftHandVel, rightHandVel;
	Vec2D headVel;

public:
	Player(std::string playerName);

	// set/get methods
	inline void setLastReceivedData(ServerToClient_Data_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	inline void setPrevReceivedData(ServerToClient_Data_TypeDef prevReceivedData) { this->prevReceivedData = prevReceivedData; };
	inline ServerToClient_Data_TypeDef& getLastReceivedData() { return lastReceivedData; };
	inline ServerToClient_Data_TypeDef& getPrevReceivedData() { return prevReceivedData; };
	inline std::string getName() { return name; };
	inline Vec2D& getLeftHandPos() { return leftHandPos; };
	inline Vec2D& getRightHandPos() { return rightHandPos; };
	inline Vec2D& getHeadPos() { return headPos; };
	inline char isPaused() { return paused; };
	inline float getLifeBar() { return lifeBar; };
	inline float getStaminaBar() { return staminaBar; };

	inline void dataAreReceived() { comTimer.start(); }; // remise à zéro du timer
	inline unsigned long long checkTime() { return comTimer.getTime(); };

	void pullLastReceivedData(); // stocke les données du dernier datagramme reçu dans les caractéristiques du joueur
	void setAutoShiftingParameters(); // met à jour les paramètres du déplacement automatique
	void updatePosAutoShifting(float headWidth, float headHeight, float handWidth, float handHeight);
};
