#include "PlayerOnline.hpp"

#include "Settings.hpp"

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

	leftHandRect = { 0, 0, 0, 0};
	rightHandRect = { 0, 0, 0, 0};
	headRect = { 0, 0, 0, 0};

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

void PlayerOnline::render(SDL_Renderer* renderer)
{
	headRect = { (int)(headPos.x * settings.screenWidth / 100), (int)(headPos.y * settings.screenHeight / 100), HEAD_WIDTH, HEAD_HEIGHT };
	leftHandRect = { (int)(leftHandPos.x * settings.screenWidth / 100), (int)(leftHandPos.y * settings.screenHeight / 100), HAND_WIDTH, HAND_HEIGHT };
	rightHandRect = { (int)(rightHandPos.x * settings.screenWidth / 100), (int)(rightHandPos.y * settings.screenHeight / 100), HAND_WIDTH, HAND_HEIGHT };

	SDL_SetRenderDrawColor(renderer, 255, 0, 0, 255);
	SDL_RenderFillRect(renderer, &headRect);

	SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
	SDL_RenderFillRect(renderer, &leftHandRect);
	SDL_RenderFillRect(renderer, &rightHandRect);
}