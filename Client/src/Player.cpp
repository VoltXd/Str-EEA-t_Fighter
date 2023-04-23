#include "Player.hpp"

Player::Player(std::string playerName) {
	name = playerName;
	prevReceivedData = { LOCAL_PLAYER_HEADING, 0, {INITIAL_LEFTHANDPOS, INITIAL_RIGHTHANDPOS}, INITIAL_HEADPOS, {INITIAL_HANDDEPTH, INITIAL_HANDDEPTH},
		INITIAL_HANDSTATE, 0, INITIAL_LIFEBAR, INITIAL_STAMINABAR, INITIAL_AFTERPUNCHDELAY };

	leftHandPos = INITIAL_LEFTHANDPOS;
	rightHandPos = INITIAL_RIGHTHANDPOS;
	headPos = INITIAL_HEADPOS;
	leftHandDepth = INITIAL_HANDDEPTH;
	rightHandDepth = INITIAL_HANDDEPTH;
	handState = INITIAL_HANDSTATE;
	paused = 0;
	lifeBar = INITIAL_LIFEBAR;
	staminaBar = INITIAL_STAMINABAR;
	afterPunchDelay = INITIAL_AFTERPUNCHDELAY;

	autoShiftingTimer.start();
}

void Player::pullLastReceivedData() {
	leftHandPos = lastReceivedData.handPos[0];
	rightHandPos = lastReceivedData.handPos[1];
	headPos = lastReceivedData.headPos;
	leftHandDepth = lastReceivedData.handDepth[0];
	rightHandDepth = lastReceivedData.handDepth[1];
	handState = lastReceivedData.handState;
	paused = lastReceivedData.paused;
	lifeBar = lastReceivedData.lifeBar;
	staminaBar = lastReceivedData.staminaBar;
	afterPunchDelay = lastReceivedData.afterPunchDelay;
}

void Player::setAutoShiftingParameters() {
	unsigned long long delayBtw2LastData = autoShiftingTimer.getTime();

	leftHandVel = (lastReceivedData.handPos[0] - prevReceivedData.handPos[0]) / delayBtw2LastData;
	rightHandVel = (lastReceivedData.handPos[1] - prevReceivedData.handPos[1]) / delayBtw2LastData;
	headVel = (lastReceivedData.headPos - prevReceivedData.headPos) / delayBtw2LastData;
	leftHandDepthVel = (lastReceivedData.handDepth[0] - prevReceivedData.handDepth[0]) / delayBtw2LastData;
	rightHandDepthVel = (lastReceivedData.handDepth[1] - prevReceivedData.handDepth[1]) / delayBtw2LastData;

	autoShiftingTimer.start();
}

void Player::updatePosAutoShifting(float handWidth, float headWidth) {
	// mise à jour des positions automatiquement
	leftHandPos += leftHandVel * autoShiftingTimer.getTime();
	LOWER_BOUND(leftHandPos, handWidth/2);
	UPPER_BOUND(leftHandPos, 100 - handWidth/2);
	rightHandPos += rightHandVel * autoShiftingTimer.getTime();
	LOWER_BOUND(rightHandPos, handWidth/2);
	UPPER_BOUND(rightHandPos, 100 - handWidth/2);
	headPos += headVel * autoShiftingTimer.getTime();
	LOWER_BOUND(headPos, headWidth / 2);
	UPPER_BOUND(headPos, 100 - headWidth / 2);
	leftHandDepth += leftHandDepthVel * autoShiftingTimer.getTime();
	LOWER_BOUND(leftHandDepth, 0);
	UPPER_BOUND(leftHandDepth, 100);
	rightHandDepth += rightHandDepthVel * autoShiftingTimer.getTime();
	LOWER_BOUND(leftHandDepth, 0);
	UPPER_BOUND(leftHandDepth, 100);
}

void Player::displayPos() {
	std::cout << "left : " << leftHandPos << " - right : " << rightHandPos << std::endl;
}