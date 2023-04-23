#include "Gameplay.hpp"

Gameplay::Gameplay(Player* player1, Player* player2)
{
	punched_P1 = false;
	punched_P2 = false;
	isOnGuard_P1 = false;
	isOnGuard_P2 = false;

	lifeBarTimer_P1.start();
	lifeBarTimer_P2.start();
	staminaBarTimer_P1.start();
	staminaBarTimer_P2.start();

	this->player1 = player1;
	this->player2 = player2;
}

void Gameplay::process_data() {
	if ((!player1->getPaused()) && (!player2->getPaused())) {
		if (isOnGuard_P1 != ((player1->getHandState() == IS_AT_THE_LEVEL_OF_GUARD) && (player1->getLeftHandDepth() <= PUNCH_DEPTH_INF_LIMIT) && (player1->getRightHandDepth() <= PUNCH_DEPTH_INF_LIMIT))) {
			staminaBarTimer_P1.start();
		}
		if (isOnGuard_P2 != ((player2->getHandState() == IS_AT_THE_LEVEL_OF_GUARD) && (player2->getLeftHandDepth() <= PUNCH_DEPTH_INF_LIMIT) && (player2->getRightHandDepth() <= PUNCH_DEPTH_INF_LIMIT))) {
			staminaBarTimer_P2.start();
		}
		isOnGuard_P1 = (player1->getHandState() == IS_AT_THE_LEVEL_OF_GUARD) && (player1->getLeftHandDepth() <= PUNCH_DEPTH_INF_LIMIT) && (player1->getRightHandDepth() <= PUNCH_DEPTH_INF_LIMIT);
		isOnGuard_P2 = (player2->getHandState() == IS_AT_THE_LEVEL_OF_GUARD) && (player2->getLeftHandDepth() <= PUNCH_DEPTH_INF_LIMIT) && (player2->getRightHandDepth() <= PUNCH_DEPTH_INF_LIMIT);

		// gestion barre stamina
		if (isOnGuard_P1) { // perte de stamina continue en garde
			if (player1->getStaminaBar() > 0) {
				player1->setStaminaBar(player1->getStaminaBar() - staminaBarTimer_P1.getTime() * STAMINA_LOSS_ON_GUARD_VELOCITY);
				staminaBarTimer_P1.start();
			}
			else {
				player1->setStaminaBar(0);
			}
		}
		else { // gain de stamina au repos
			if (player1->getStaminaBar() < 100) { 
				player1->setStaminaBar(player1->getStaminaBar() + staminaBarTimer_P1.getTime() * STAMINA_IDLE_GROWTH_VELOCITY);
				staminaBarTimer_P1.start();
			}
			else {
				player1->setStaminaBar(100);
			}
		}
		if (isOnGuard_P2) { // perte de stamina continue en garde
			if (player2->getStaminaBar() > 0) {
				player2->setStaminaBar(player2->getStaminaBar() - staminaBarTimer_P2.getTime() * STAMINA_LOSS_ON_GUARD_VELOCITY);
				staminaBarTimer_P2.start();
			}
			else {
				player2->setStaminaBar(0);
			}
		}
		else { // gain de stamina au repos
			if (player2->getStaminaBar() < 100) {
				player2->setStaminaBar(player2->getStaminaBar() + staminaBarTimer_P2.getTime() * STAMINA_IDLE_GROWTH_VELOCITY);
				staminaBarTimer_P2.start();
			}
			else {
				player2->setStaminaBar(100);
			}
		}

		// gestion croissance barre de vie des deux joueurs
		if (player1->getLifeBar() > 0) {
			if (player1->getLifeBar() < 100) {
				player1->setLifeBar(player1->getLifeBar() + lifeBarTimer_P1.getTime() * LIFE_BAR_GROWTH_VELOCITY);
				lifeBarTimer_P1.start();
			}
			else {
				player1->setLifeBar(100);
			}
		}
		if (player2->getLifeBar() > 0) {
			if (player2->getLifeBar() < 100) {
				player2->setLifeBar(player2->getLifeBar() + lifeBarTimer_P2.getTime() * LIFE_BAR_GROWTH_VELOCITY);
				lifeBarTimer_P2.start();
			}
			else {
				player2->setLifeBar(100);
			}
		}

		// gestion coup player1 sur player2
		if ((player1->getLeftHandDepth() >= PUNCH_DEPTH_SUP_LIMIT) && (!punched_P1)) { // coup donné
			if ((player1->getLeftHandPos() >= player2->getHeadPos() - HEAD_POS_TOLERANCE) && (player1->getLeftHandPos() <= player2->getHeadPos() + HEAD_POS_TOLERANCE)) {
				if (!isOnGuard_P2) {
					if (player2->getLifeBar() > HP_LOSS) {
						player2->setLifeBar(player2->getLifeBar() - HP_LOSS);
					}
					else {
						player2->setLifeBar(0);
					}
					lifeBarTimer_P2.start();
				}
			}
			if (player1->getStaminaBar() > STAMINA_LOSS_PER_PUNCH) {
				player1->setStaminaBar(player1->getStaminaBar() - STAMINA_LOSS_PER_PUNCH);
				staminaBarTimer_P1.start();
			}
			punched_P1 = true;
		}
		if ((player1->getRightHandDepth() >= PUNCH_DEPTH_SUP_LIMIT) && (!punched_P1)) {
			if ((player1->getRightHandPos() >= player2->getHeadPos() - HEAD_POS_TOLERANCE) && (player1->getRightHandPos() <= player2->getHeadPos() + HEAD_POS_TOLERANCE)) {
				if (!isOnGuard_P2) {
					if (player2->getLifeBar() > HP_LOSS) {
						player2->setLifeBar(player2->getLifeBar() - HP_LOSS);
					}
					else {
						player2->setLifeBar(0);
					}
					lifeBarTimer_P2.start();
				}
			}
			if (player1->getStaminaBar() > STAMINA_LOSS_PER_PUNCH) {
				player1->setStaminaBar(player1->getStaminaBar() - STAMINA_LOSS_PER_PUNCH);
				staminaBarTimer_P1.start();
			}
			punched_P1 = true;
		}

		// gestion coup player2 sur player1
		if ((player2->getLeftHandDepth() >= PUNCH_DEPTH_SUP_LIMIT) && (!punched_P2)) { // coup donné
			if ((player2->getLeftHandPos() >= player1->getHeadPos() - HEAD_POS_TOLERANCE) && (player2->getLeftHandPos() <= player1->getHeadPos() + HEAD_POS_TOLERANCE)) {
				if (!isOnGuard_P1)	{
					if (player1->getLifeBar() > HP_LOSS) {
						player1->setLifeBar(player1->getLifeBar() - HP_LOSS);
					}
					else {
						player1->setLifeBar(0);
					}
					lifeBarTimer_P1.start();
				}
			}
			if (player2->getStaminaBar() > STAMINA_LOSS_PER_PUNCH) {
				player2->setStaminaBar(player2->getStaminaBar() - STAMINA_LOSS_PER_PUNCH);
				staminaBarTimer_P2.start();
			}
			punched_P2 = true;
		}
		if ((player2->getRightHandDepth() >= PUNCH_DEPTH_SUP_LIMIT) && (!punched_P2)) {
			if ((player2->getRightHandPos() >= player1->getHeadPos() - HEAD_POS_TOLERANCE) && (player2->getRightHandPos() <= player1->getHeadPos() + HEAD_POS_TOLERANCE)) {
				if (!isOnGuard_P1) {
					if (player1->getLifeBar() > HP_LOSS) {
						player1->setLifeBar(player1->getLifeBar() - HP_LOSS);
					}
					else {
						player1->setLifeBar(0);
					}
					lifeBarTimer_P1.start();
				}
			}
			if (player2->getStaminaBar() > STAMINA_LOSS_PER_PUNCH) {
				player2->setStaminaBar(player2->getStaminaBar() - STAMINA_LOSS_PER_PUNCH);
				staminaBarTimer_P2.start();
			}
			punched_P2 = true;
		}

		// coup à nouveau possible à donner
		if ((player1->getLeftHandDepth() <= PUNCH_DEPTH_SUP_LIMIT) && (player1->getRightHandDepth() <= PUNCH_DEPTH_SUP_LIMIT)) {
			punched_P1 = false;
		}
		if ((player2->getLeftHandDepth() <= PUNCH_DEPTH_SUP_LIMIT) && (player2->getRightHandDepth() <= PUNCH_DEPTH_SUP_LIMIT)) {
			punched_P2 = false;
		}
	}
}