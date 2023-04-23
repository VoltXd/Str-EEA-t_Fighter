#pragma once

/* --- Class Player --- 
* les attributs en float correspondent � des pourcentages
*/

#include <iostream>
#include <string>

#include <WinSock2.h> 

#include "Data_structs.hpp"
#include "Timer.hpp"

class Player {
private :
	std::string name;
	SOCKADDR_IN addr;
	ClientToServer_Position_TypeDef lastReceivedData; // derni�res donn�es correctes re�ues
	Timer<std::chrono::milliseconds> comLossesTimer; // timer pour la gestion de la perte de communication

	float leftHandPos, rightHandPos;
	float headPos;
	float leftHandDepth, rightHandDepth;
	char handState;
	char paused;
	float lifeBar;
	float staminaBar;
	float afterPunchDelay;

	unsigned long long dateOfLastPullData;

public :
	Player();

	// set/get methods
	void setName(const char* name) { this->name = std::string(name); };
	void setName(const std::string& name) { this->name = std::string(name); };
	void setAddr(SOCKADDR_IN addr) { this->addr = addr; };
	void setLastReceivedData(ClientToServer_Position_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	void setLifeBar(float lifeBar) { this->lifeBar = lifeBar; };
	void setStaminaBar(float staminaBar) { this->staminaBar = staminaBar; };
	std::string getName() { return name; };
	SOCKADDR_IN& getAddr() { return addr; };
	float getLeftHandPos() { return leftHandPos; };
	float getRightHandPos() { return rightHandPos; };
	float getHeadPos() { return headPos; };
	char getHandState() { return handState; };
	char getPaused() { return paused; };
	float getLeftHandDepth() { return leftHandDepth; };
	float getRightHandDepth() { return rightHandDepth; };
	float getLifeBar() { return lifeBar; };
	float getStaminaBar() { return staminaBar; };
	float getAfterPunchDelay() { return afterPunchDelay; };

	void dataAreReceived() { comLossesTimer.start(); }; // remise � z�ro du timer
	unsigned long long checkTime() { return comLossesTimer.getTime(); };

	void sendStartDatagram(SOCKET& socket, std::string opponentName); // envoi du datagramme pour le d�marrage du jeu
	void pullLastReceivedData(); // stocke les donn�es du dernier datagramme re�u
	void pushCurrentPlayerData(SOCKET& socket, SOCKADDR_IN opponentAddr); // envoie les donn�es actuelle de cette instance au joueur associ� et � l'adversaire

	void displayPos();
};

