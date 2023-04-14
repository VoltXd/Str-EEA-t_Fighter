#include "Player.hpp"

Player::Player() {
	name = "undefined";
	addr = { 0 };
	lastReceivedData = { POSITION_HEADING, 0, {INITIAL_LEFTHANDPOS, INITIAL_RIGHTHANDPOS}, INITIAL_HEADPOS, 
		INITIAL_PUNCHDEPTH, INITIAL_HANDSTATE , 0}; // position initiale

	leftHandPos = INITIAL_LEFTHANDPOS;
	rightHandPos = INITIAL_RIGHTHANDPOS;
	headPos = INITIAL_HEADPOS;
	punchDepth = INITIAL_PUNCHDEPTH;
	handState = INITIAL_HANDSTATE;
	paused = 0;
	lifeBar = INITIAL_LIFEBAR;
	guardBar = INITIAL_GUARDBAR;
	afterPunchDelay = INITIAL_AFTERPUNCHDELAY;

	dateOfLastPullData = ~0; // temps tr�s grand tant qu'on a pas re�u la premi�re trame de donn�es
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
	punchDepth = lastReceivedData.punchDepth;
	handState = lastReceivedData.handState;
	paused = lastReceivedData.paused;
}

void Player::pushCurrentPlayerData(SOCKET& socket, SOCKADDR_IN opponentAddr) {
	//SOCKADDR_IN receiverAddr;
	ServerToClient_Data_TypeDef playerData;

	playerData.date = dateOfLastPullData;
	playerData.handPos[0] = getLeftHandPos();
	playerData.handPos[1] = getRightHandPos();
	playerData.headPos = getHeadPos();
	playerData.punchDepth = getPunchDepth();
	playerData.handState = getHandState();
	playerData.paused = getPaused();
	playerData.lifeBar = getLifeBar();
	playerData.guardBar = getGuardBar();
	playerData.afterPunchDelay = getAfterPunchDelay();

	// envoi au joueur lui-m�me
	playerData.heading = LOCAL_PLAYER_HEADING;
	sendto(socket, (char*)&playerData, sizeof(playerData), 0, (SOCKADDR*)&addr, sizeof(addr));

	// envoi au joueur adverse
	playerData.heading = OPPOSING_PLAYER_HEADING;
	sendto(socket, (char*)&playerData, sizeof(playerData), 0, (SOCKADDR*)&opponentAddr, sizeof(opponentAddr));
}

void Player::displayPos() {
	std::cout << "left : " << leftHandPos << " - right : " << rightHandPos << std::endl;
}