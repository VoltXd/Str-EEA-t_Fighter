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
private :
	string name;
	SOCKADDR_IN addr;
	ClientToServer_Position_TypeDef lastReceivedData; // derni�res donn�es correctes re�ues
	Timer comLossesTimer; // timer pour la gestion de la perte de communication

	float leftHandPos, rightHandPos;
	float headPos;
	float punchDepth;
	char handState;
	char paused;

	float lifeBar;
	float guardBar;

	float afterPunchDelay;

public :
	Player();

	// set/get methods
	void setName(const char* name) { this->name = name; };
	void setName(const string name) { this->name = name; };
	void setAddr(SOCKADDR_IN& addr) { this->addr = addr; };
	void setLastReceivedData(ClientToServer_Position_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	void setLifeBar(float lifeBar) { this->lifeBar = lifeBar; };
	void setGuardBar(float guardBar) { this->guardBar = guardBar; };
	string getName() { return name; };
	SOCKADDR_IN& getAddr() { return addr; };
	float getLeftHandPos() { return leftHandPos; };
	float getRightHandPos() { return rightHandPos; };
	float getHeadPos() { return headPos; };
	char getHandState() { return handState; };
	char getPaused() { return paused; };
	float getPunchDepth() { return punchDepth; };
	float getLifeBar() { return lifeBar; };
	float getGuardBar() { return guardBar; };
	float getAfterPunchDelay() { return afterPunchDelay; };

	void dataAreReceived() { comLossesTimer.start(); }; // remise � z�ro du timer
	double checkTime() { return comLossesTimer.getTime(); }; 

	void sendStartDatagram(SOCKET& socket, string opponentName); // envoi du datagramme pour le d�marrage du jeu
	void pullLastReceivedData(); // stocke les donn�es du dernier datagramme re�u
	void pushCurrentPlayerData(SOCKET& socket, SOCKADDR_IN opponentAddr); // envoie les donn�es actuelle de cette instance au joueur associ� et � l'adversaire

	void displayPos();
};

