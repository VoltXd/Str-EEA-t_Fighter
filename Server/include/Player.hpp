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

class Player {
private :
	std::string name;
	SOCKADDR_IN addr;
	ClientToServer_Position_TypeDef lastReceivedData; // dernières données correctes reçues
	Timer<std::chrono::milliseconds> comLossesTimer; // timer pour la gestion de la perte de communication

	Vec2D leftHandPos, rightHandPos;
	Vec2D headPos;
	char paused;
	float lifeBar;
	float staminaBar;

	bool onGuard;
	bool punched; // vrai lorsque le coup vient juste d'être donné

	unsigned long long dateOfLastPullData;

public :
	Player();

	// set/get methods
	inline void setName(const char* name) { this->name = std::string(name); };
	inline void setName(const std::string& name) { this->name = std::string(name); };
	inline void setAddr(SOCKADDR_IN addr) { this->addr = addr; };
	inline void setLastReceivedData(ClientToServer_Position_TypeDef lastReceivedData) { this->lastReceivedData = lastReceivedData; };
	inline void setLifeBar(float lifeBar) { this->lifeBar = lifeBar; };
	inline void setStaminaBar(float staminaBar) { this->staminaBar = staminaBar; };
	inline void setGuard(bool onGuard) { this->onGuard = onGuard; };
	inline void setPunch(bool punched) { this->punched = punched; };

	inline std::string getName() { return name; };
	inline SOCKADDR_IN& getAddr() { return addr; };
	inline Vec2D& getLeftHandPos() { return leftHandPos; };
	inline Vec2D& getRightHandPos() { return rightHandPos; };
	inline Vec2D& getHeadPos() { return headPos; };
	inline char isPaused() { return paused; };
	inline float getLifeBar() { return lifeBar; };
	inline float getStaminaBar() { return staminaBar; };
	inline bool isOnGuard() { return onGuard; };
	inline bool hasPunched() { return punched; };

	inline void dataAreReceived() { comLossesTimer.start(); }; // remise à zéro du timer
	inline unsigned long long checkTime() { return comLossesTimer.getTime(); };

	void sendStartDatagram(SOCKET& socket, std::string opponentName); // envoi du datagramme pour le démarrage du jeu
	void pullLastReceivedData(); // stocke les données du dernier datagramme reçu
	void pushCurrentPlayerData(SOCKET& socket, SOCKADDR_IN opponentAddr); // envoie les données actuelle de cette instance au joueur associé et à l'adversaire

};

