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
	long delayBtw2LastData; // d�lai entre les deux derni�res donn�es re�ues 
	Timer comLossesTimer; // timer pour la gestion de la perte de communication
	Timer autoShiftingTimer; // timer pour le d�placement automatique

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

	void dataAreReceived() { comLossesTimer.start(); autoShiftingTimer.start(); }; // remise � z�ro du timer
	long checkTime() { return comLossesTimer.getTime(); };

	void pullLastReceivedData(); // stocke les donn�es du dernier datagramme re�u dans les caract�ristiques du joueur
	void updatePosAutoShifting(float handWidth, float headWidth);

	void displayPos();
};
