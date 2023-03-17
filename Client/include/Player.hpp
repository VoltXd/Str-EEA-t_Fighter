#pragma once

/* --- Class Player ---
* les attributs en float correspondent � des pourcentages
*/

#include <iostream>
#include <string>
using namespace std;

#include <WinSock2.h> 

#include "Data_structs.hpp"
#include "Timer.hpp"

class Player {
private:
	string name;
	ServerToClient_Data_TypeDef lastReceivedData; // derni�res donn�es correctes re�ues
	Timer comLossesTimer; // timer pour la gestion de la perte de communication

	float leftHandPos, rightHandPos;
	float headPos;
	float punchDepth;
	char handState;

	float lifeBar;
	float guardBar;

	float afterPunchDelay;

public:
	Player(string playerName);

	// set/get methods
	void setLastReceivedData(ServerToClient_Data_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	string getName() { return name; };
	float getLeftHandPos() { return leftHandPos; };
	float getRightHandPos() { return rightHandPos; };
	float getHeadPos() { return headPos; };
	float getPunchDepth() { return punchDepth; };
	float getLifeBar() { return lifeBar; };
	float getGuardBar() { return guardBar; };

	void dataAreReceived() { comLossesTimer.start(); }; // remise � z�ro du timer
	double checkTime() { return comLossesTimer.getTime(); };

	void pullLastReceivedData(); // stocke les donn�es du dernier datagramme re�u

	void displayPos();
};
