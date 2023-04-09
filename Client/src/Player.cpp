#include "Player.hpp"

Player::Player(std::string playerName) {
	name = playerName;
	prevReceivedData = { LOCAL_PLAYER_HEADING, 0, {INITIAL_LEFTHANDPOS, INITIAL_RIGHTHANDPOS}, INITIAL_HEADPOS, INITIAL_PUNCHDEPTH,
		INITIAL_HANDSTATE, 0, INITIAL_LIFEBAR, INITIAL_GUARDBAR, INITIAL_AFTERPUNCHDELAY };

	leftHandPos = INITIAL_LEFTHANDPOS;
	rightHandPos = INITIAL_RIGHTHANDPOS;
	headPos = INITIAL_HEADPOS;
	punchDepth = INITIAL_PUNCHDEPTH;
	handState = INITIAL_HANDSTATE;
	paused = 0;
	lifeBar = INITIAL_LIFEBAR;
	guardBar = INITIAL_GUARDBAR;
	afterPunchDelay = INITIAL_AFTERPUNCHDELAY;

	autoShiftingTimer.start();
}

void Player::pullLastReceivedData() {
	leftHandPos = lastReceivedData.handPos[0];
	rightHandPos = lastReceivedData.handPos[1];
	headPos = lastReceivedData.headPos;
	punchDepth = lastReceivedData.punchDepth;
	handState = lastReceivedData.handState;
	paused = lastReceivedData.paused;
	lifeBar = lastReceivedData.lifeBar;
	guardBar = lastReceivedData.guardBar;
	afterPunchDelay = lastReceivedData.afterPunchDelay;
}

void Player::setAutoShiftingParameters() {
	unsigned long long delayBtw2LastData = autoShiftingTimer.getTime();

	leftHandVel = (lastReceivedData.handPos[0] - prevReceivedData.handPos[0]) / delayBtw2LastData;
	rightHandVel = (lastReceivedData.handPos[1] - prevReceivedData.handPos[1]) / delayBtw2LastData;
	headVel = (lastReceivedData.headPos - prevReceivedData.headPos) / delayBtw2LastData;
	punchVel = (lastReceivedData.punchDepth - prevReceivedData.punchDepth) / delayBtw2LastData;

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
	punchDepth += punchVel * autoShiftingTimer.getTime();
	LOWER_BOUND(punchDepth, 0);
	UPPER_BOUND(punchDepth, 100);
}

void Player::displayPos() {
	std::cout << "left : " << leftHandPos << " - right : " << rightHandPos << std::endl;
}