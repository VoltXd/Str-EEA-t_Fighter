#include "Player.hpp"

Player::Player() {
	name = "undefined";
	addr = { 0 };
	lastReceivedData = { POSITION_HEADING, 0, {INITIAL_LEFTHANDPOS, INITIAL_RIGHTHANDPOS}, INITIAL_HEADPOS, INITIAL_GAMESTATE }; // position initiale

	leftHandPos = INITIAL_LEFTHANDPOS;
	rightHandPos = INITIAL_RIGHTHANDPOS;
	headPos = INITIAL_HEADPOS;
	paused = INITIAL_GAMESTATE;
	lifeBar = INITIAL_LIFEBAR;
	staminaBar = INITIAL_STAMINABAR;

	onGuard = false;
	punched = false;

	dateOfLastPullData = ~0; // temps très grand tant qu'on a pas reçu la première trame de données
}

void Player::sendStartDatagram(SOCKET& socket, std::string opponentName) {
	ServerToClient_Start_TypeDef startData;
	sprintf_s(startData.opponentName, "%s", opponentName.c_str());
	sendto(socket, (char*)&startData, sizeof(startData), 0, (SOCKADDR*)&addr, sizeof(addr));
}

void Player::pullLastReceivedData() {
	dateOfLastPullData = lastReceivedData.date;
	leftHandPos = lastReceivedData.handPos[0];
	rightHandPos = lastReceivedData.handPos[1];
	headPos = lastReceivedData.headPos;
	paused = lastReceivedData.paused;
}

void Player::pushCurrentPlayerData(SOCKET& socket, SOCKADDR_IN opponentAddr) {
	//SOCKADDR_IN receiverAddr;
	ServerToClient_Data_TypeDef playerData;

	playerData.date = dateOfLastPullData;
	playerData.handPos[0] = getLeftHandPos();
	playerData.handPos[1] = getRightHandPos();
	playerData.headPos = getHeadPos();
	playerData.paused = isPaused();
	playerData.lifeBar = getLifeBar();
	playerData.staminaBar = getStaminaBar();

	// envoi au joueur lui-même
	playerData.heading = LOCAL_PLAYER_HEADING;
	sendto(socket, (char*)&playerData, sizeof(playerData), 0, (SOCKADDR*)&addr, sizeof(addr));

	// envoi au joueur adverse
	playerData.heading = OPPOSING_PLAYER_HEADING;
	sendto(socket, (char*)&playerData, sizeof(playerData), 0, (SOCKADDR*)&opponentAddr, sizeof(opponentAddr));
}
