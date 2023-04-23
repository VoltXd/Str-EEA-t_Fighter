#pragma once

/* --- Class Player ---
* les attributs en float correspondent � des pourcentages
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
	ServerToClient_Data_TypeDef lastReceivedData; // derni�res donn�es correctes re�ues
	ServerToClient_Data_TypeDef prevReceivedData; // avant-derni�res donn�es correctes re�ues
	Timer<std::chrono::milliseconds> comTimer; // timer pour la gestion de la communication par rapport au temps
	Timer<std::chrono::microseconds> autoShiftingTimer; // timer pour la gestion du d�placement automatique

	float leftHandPos, rightHandPos;
	float headPos;
	float leftHandDepth;
	float rightHandDepth;
	char handState;
	char paused;

	float lifeBar;
	float staminaBar;

	float afterPunchDelay;

	// vitesses pour le d�placement automatique
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

	void dataAreReceived() { comTimer.start(); }; // remise � z�ro du timer
	unsigned long long checkTime() { return comTimer.getTime(); };

	void pullLastReceivedData(); // stocke les donn�es du dernier datagramme re�u dans les caract�ristiques du joueur
	void setAutoShiftingParameters(); // met � jour les param�tres du d�placement automatique
	void updatePosAutoShifting(float handWidth, float headWidth);

	void displayPos();
};
