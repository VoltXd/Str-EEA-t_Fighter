#include "Player.hpp"

Player::Player(string playerName) {
	name = playerName;
	lastReceivedData = { LOCAL_PLAYER_HEADING, {INITIAL_LEFTHANDPOS, INITIAL_RIGHTHANDPOS}, INITIAL_HEADPOS, INITIAL_PUNCHDEPTH,
		INITIAL_HANDSTATE, INITIAL_LIFEBAR, INITIAL_GUARDBAR, INITIAL_AFTERPUNCHDELAY };

	leftHandPos = INITIAL_LEFTHANDPOS;
	rightHandPos = INITIAL_RIGHTHANDPOS;
	headPos = INITIAL_HEADPOS;
	punchDepth = INITIAL_PUNCHDEPTH;
	handState = INITIAL_HANDSTATE;
	lifeBar = INITIAL_LIFEBAR;
	guardBar = INITIAL_GUARDBAR;
	afterPunchDelay = INITIAL_AFTERPUNCHDELAY;
}

void Player::pullLastReceivedData() {
	leftHandPos = lastReceivedData.handPos[0];
	rightHandPos = lastReceivedData.handPos[1];
	headPos = lastReceivedData.headPos;
	punchDepth = lastReceivedData.punchDepth;
	handState = lastReceivedData.handState;
	lifeBar = lastReceivedData.lifeBar;
	guardBar = lastReceivedData.guardBar;
	afterPunchDelay = lastReceivedData.afterPunchDelay;
}

void Player::displayPos() {
	cout << "left : " << leftHandPos << " - right : " << rightHandPos << endl;
}