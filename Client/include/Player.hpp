#pragma once

/* --- Class Player ---
* les attributs en float correspondent à des pourcentages
*/

#include <iostream>
#include <string>

#include <WinSock2.h> 

#include "Data_structs.hpp"
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

	float leftHandPos, rightHandPos;
	float headPos;
	float leftHandDepth;
	float rightHandDepth;
	char handState;
	char paused;

	float lifeBar;
	float staminaBar;

	float afterPunchDelay;

	// vitesses pour le déplacement automatique
	float leftHandVel;
	float rightHandVel;
	float headVel;
	float leftHandDepthVel;
	float rightHandDepthVel;

public:
	Player(std::string playerName);

	// set/get methods
	void setLastReceivedData(ServerToClient_Data_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	void setPrevReceivedData(ServerToClient_Data_TypeDef prevReceivedData) { this->prevReceivedData = prevReceivedData; };
	ServerToClient_Data_TypeDef& getLastReceivedData() { return lastReceivedData; };
	ServerToClient_Data_TypeDef& getPrevReceivedData() { return prevReceivedData; };
	std::string getName() { return name; };
	float getLeftHandPos() { return leftHandPos; };
	float getRightHandPos() { return rightHandPos; };
	float getHeadPos() { return headPos; };
	float getLeftHandDepth() { return leftHandDepth; };
	float getRightHandDepth() { return rightHandDepth; };
	float getLifeBar() { return lifeBar; };
	float getStaminaBar() { return staminaBar; };

	void dataAreReceived() { comTimer.start(); }; // remise à zéro du timer
	unsigned long long checkTime() { return comTimer.getTime(); };

	void pullLastReceivedData(); // stocke les données du dernier datagramme reçu dans les caractéristiques du joueur
	void setAutoShiftingParameters(); // met à jour les paramètres du déplacement automatique
	void updatePosAutoShifting(float handWidth, float headWidth);

	void displayPos();
};
