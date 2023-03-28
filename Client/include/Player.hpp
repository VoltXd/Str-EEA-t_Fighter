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
	long delayBtw2LastData; // délai entre les deux dernières données reçues 
	Timer comLossesTimer; // timer pour la gestion de la perte de communication
	Timer autoShiftingTimer; // timer pour le déplacement automatique

	float leftHandPos, rightHandPos;
	float headPos;
	float punchDepth;
	char handState;
	char paused;

	float lifeBar;
	float guardBar;

	float afterPunchDelay;

public:
	Player(std::string playerName);

	// set/get methods
	void setLastReceivedData(ServerToClient_Data_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	void setPrevReceivedData(ServerToClient_Data_TypeDef prevReceivedData) { this->prevReceivedData = prevReceivedData; };
	void setDelayBtw2LastData(long delayBtw2LastData) { this->delayBtw2LastData = delayBtw2LastData; };
	ServerToClient_Data_TypeDef getLastReceivedData() { return lastReceivedData; };
	std::string getName() { return name; };
	float getLeftHandPos() { return leftHandPos; };
	float getRightHandPos() { return rightHandPos; };
	float getHeadPos() { return headPos; };
	float getPunchDepth() { return punchDepth; };
	float getLifeBar() { return lifeBar; };
	float getGuardBar() { return guardBar; };

	void dataAreReceived() { comLossesTimer.start(); autoShiftingTimer.start(); }; // remise à zéro du timer
	long checkTime() { return comLossesTimer.getTime(); };

	void pullLastReceivedData(); // stocke les données du dernier datagramme reçu dans les caractéristiques du joueur
	void updatePosAutoShifting(float handWidth, float headWidth);

	void displayPos();
};
