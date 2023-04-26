#include "PlayerOnline.hpp"

PlayerOnline::PlayerOnline(std::string playerName) {
	name = playerName;
	prevReceivedData = { LOCAL_PLAYER_HEADING, 0, {INITIAL_LEFTHANDPOS, INITIAL_RIGHTHANDPOS}, INITIAL_HEADPOS, INITIAL_GAMESTATE, 
		INITIAL_LIFEBAR, INITIAL_STAMINABAR};

	leftHandPos = INITIAL_LEFTHANDPOS;
	rightHandPos = INITIAL_RIGHTHANDPOS;
	headPos = INITIAL_HEADPOS;
	paused = INITIAL_GAMESTATE;
	lifeBar = INITIAL_LIFEBAR;
	staminaBar = INITIAL_STAMINABAR;

	autoShiftingTimer.start();
}

void PlayerOnline::pullLastReceivedData() {
	leftHandPos = lastReceivedData.handPos[0];
	rightHandPos = lastReceivedData.handPos[1];
	headPos = lastReceivedData.headPos;
	paused = lastReceivedData.paused;
	lifeBar = lastReceivedData.lifeBar;
	staminaBar = lastReceivedData.staminaBar;
}

void PlayerOnline::setAutoShiftingParameters() {
	unsigned long long delayBtw2LastData = autoShiftingTimer.getTime();

	leftHandVel = (lastReceivedData.handPos[0] - prevReceivedData.handPos[0]) / delayBtw2LastData;
	rightHandVel = (lastReceivedData.handPos[1] - prevReceivedData.handPos[1]) / delayBtw2LastData;
	headVel = (lastReceivedData.headPos - prevReceivedData.headPos) / delayBtw2LastData;

	autoShiftingTimer.start();
}

void PlayerOnline::updatePosAutoShifting(float headWidth, float headHeight, float handWidth, float handHeight)	{
// mise � jour des positions automatiquement
	leftHandPos = leftHandPos + leftHandVel * autoShiftingTimer.getTime();
	LOWER_BOUND(leftHandPos.x, handWidth/2);
	UPPER_BOUND(leftHandPos.x, 100 - handWidth/2);
	LOWER_BOUND(leftHandPos.y, handHeight / 2);
	UPPER_BOUND(leftHandPos.y, 100 - handHeight / 2);
	rightHandPos = rightHandPos + rightHandVel * autoShiftingTimer.getTime();
	LOWER_BOUND(rightHandPos.x, handWidth / 2);
	UPPER_BOUND(rightHandPos.x, 100 - handWidth / 2);
	LOWER_BOUND(rightHandPos.y, handHeight / 2);
	UPPER_BOUND(rightHandPos.y, 100 - handHeight / 2);
	headPos = headPos + headVel * autoShiftingTimer.getTime();
	LOWER_BOUND(headPos.x, headWidth / 2);
	UPPER_BOUND(headPos.x, 100 - headWidth / 2);
	LOWER_BOUND(headPos.y, headHeight / 2);
	UPPER_BOUND(headPos.y, 100 - headHeight / 2);
}
