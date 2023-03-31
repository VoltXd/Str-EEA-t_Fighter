#pragma once

/* --- Class Player --- 
* les attributs en float correspondent à des pourcentages
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
	ClientToServer_Position_TypeDef lastReceivedData; // dernières données correctes reçues
	Timer comLossesTimer; // timer pour la gestion de la perte de communication

	float leftHandPos, rightHandPos;
	float headPos;
	float punchDepth;
	char handState;
	char paused;
	float lifeBar;
	float guardBar;
	float afterPunchDelay;

	long dateOfLastPullData;

public :
	Player();

	// set/get methods
	inline void setName(const char* name) { this->name = std::string(name); };
	inline void setName(const std::string& name) { this->name = std::string(name); };
	inline void setAddr(SOCKADDR_IN addr) { this->addr = addr; };
	inline void setLastReceivedData(ClientToServer_Position_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	inline void setLifeBar(float lifeBar) { this->lifeBar = lifeBar; };
	inline void setGuardBar(float guardBar) { this->guardBar = guardBar; };
	inline std::string getName() { return name; };
	inline SOCKADDR_IN& getAddr() { return addr; };
	inline float getLeftHandPos() { return leftHandPos; };
	inline float getRightHandPos() { return rightHandPos; };
	inline float getHeadPos() { return headPos; };
	inline char getHandState() { return handState; };
	inline char getPaused() { return paused; };
	inline float getPunchDepth() { return punchDepth; };
	inline float getLifeBar() { return lifeBar; };
	inline float getGuardBar() { return guardBar; };
	inline float getAfterPunchDelay() { return afterPunchDelay; };

	inline void dataAreReceived() { comLossesTimer.start(); }; // remise à zéro du timer
	inline long checkTime() { return comLossesTimer.getTime(); };

	void sendStartDatagram(SOCKET& socket, std::string opponentName); // envoi du datagramme pour le démarrage du jeu
	void pullLastReceivedData(); // stocke les données du dernier datagramme reçu
	void pushCurrentPlayerData(SOCKET& socket, SOCKADDR_IN opponentAddr); // envoie les données actuelle de cette instance au joueur associé et à l'adversaire

	void displayPos();
};

