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

void Player::updatePosAutoShifting(float handWidth, float headWidth) {
	Timer autoShiftingTimer; // timer pour le déplacement automatique

	float leftHandVel = (lastReceivedData.handPos[0] - prevReceivedData.handPos[0]) / (float)delayBtw2LastData;
	float rightHandVel = (lastReceivedData.handPos[1] - prevReceivedData.handPos[1]) / (float)delayBtw2LastData;
	float headVel = (lastReceivedData.headPos - prevReceivedData.headPos) / (float)delayBtw2LastData;
	float punchVel = (lastReceivedData.punchDepth - prevReceivedData.punchDepth) / (float)delayBtw2LastData;

	if (!autoShiftingTimer.isStarted()) {
		autoShiftingTimer.start();
	}

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
	cout << "left : " << leftHandPos << " - right : " << rightHandPos << endl;
}