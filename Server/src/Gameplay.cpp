#include "Gameplay.hpp"

Gameplay::Gameplay(Player* player1, Player* player2)
{
	this->player1 = player1;
	this->player2 = player2;
}

inline bool Gameplay::isInRectZone(const Vec2D& point, const Vec2D& rectCenter, float rectW, float rectH) {
	return ((point.x >= rectCenter.x - rectW / 2) && (point.x <= rectCenter.x + rectW / 2) &&
		(point.y >= rectCenter.y - rectH / 2) && (point.x <= rectCenter.x + rectH / 2));
}

void Gameplay::process_data() {
	if ((!player1->isPaused()) && (!player2->isPaused())) {
		// calcul des différents seuils utiles
		// seuils pour la prise en compte du coup
		float punchLimit_P1 = player1->getHeadPos().y - PUNCH_DEPTH_SUP_LIMIT * PUNCH_DEPTH_RANGE;
		float punchLimit_P2 = player2->getHeadPos().y - PUNCH_DEPTH_SUP_LIMIT * PUNCH_DEPTH_RANGE;
		// seuils de tolérance pour les coups de chaque côté de la tête 
		float headPosLeftLimit_P1 = player1->getHeadPos().x - HEAD_POS_TOLERANCE;
		float headPosRightLimit_P1 = player1->getHeadPos().x + HEAD_POS_TOLERANCE;
		float headPosLeftLimit_P2 = player2->getHeadPos().x - HEAD_POS_TOLERANCE;
		float headPosRightLimit_P2 = player2->getHeadPos().x + HEAD_POS_TOLERANCE;
		// seuils de limite repos/non repos (seuil de garde inférieur)
		float idleVThreshold_P1 = player1->getHeadPos().y + HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE; 
		float idleVThreshold_P2 = player2->getHeadPos().y + HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE;
		// seuils de garde supérieur
		float supGuardVThreshold_P1 = player1->getHeadPos().y - HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE;
		float supGuardVThreshold_P2 = player2->getHeadPos().y - HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE;


		// calcul de l'état de garde
		player1->setGuard(isInRectZone(player1->getLeftHandPos(), player1->getHeadPos(), HEAD_WIDTH*HEAD_WIDTH_PROPORTION_GUARD_ZONE, HEAD_HEIGHT*HEAD_HEIGHT_PROPORTION_GUARD_ZONE)
		&& isInRectZone(player1->getRightHandPos(), player1->getHeadPos(), HEAD_WIDTH * HEAD_WIDTH_PROPORTION_GUARD_ZONE, HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE));
		player2->setGuard(isInRectZone(player2->getLeftHandPos(), player2->getHeadPos(), HEAD_WIDTH * HEAD_WIDTH_PROPORTION_GUARD_ZONE, HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE)
			&& isInRectZone(player2->getRightHandPos(), player2->getHeadPos(), HEAD_WIDTH * HEAD_WIDTH_PROPORTION_GUARD_ZONE, HEAD_HEIGHT * HEAD_HEIGHT_PROPORTION_GUARD_ZONE));

		// gestion coup player1 sur player2
		if (((player1->getLeftHandPos().y <= punchLimit_P1) || (player1->getRightHandPos().y <= punchLimit_P1)) && (!player1->hasPunched())) { // coup donné
			if (((player1->getLeftHandPos().x >= headPosLeftLimit_P2) && (player1->getLeftHandPos().x <= headPosRightLimit_P2))
			|| ((player1->getRightHandPos().x >= headPosLeftLimit_P2) && (player1->getRightHandPos().x <= headPosRightLimit_P2))) {
				if ((!player2->isOnGuard()) && (player1->getStaminaBar() >= STAMINA_LOSS_PER_PUNCH)) {
					if (player2->getLifeBar() > HP_LOSS_PER_PUNCH) {
						player2->setLifeBar(player2->getLifeBar() - HP_LOSS_PER_PUNCH);
					}
					else {
						player2->setLifeBar(0);
					}
				}
			}
			if (player1->getStaminaBar() >= STAMINA_LOSS_PER_PUNCH) {
				player1->setStaminaBar(player1->getStaminaBar() - STAMINA_LOSS_PER_PUNCH);
			}
			player1->setPunch(true);
		}

		// gestion coup player2 sur player1
		if (((player2->getLeftHandPos().y <= punchLimit_P2) || (player2->getRightHandPos().y <= punchLimit_P2)) && (!player2->hasPunched())) { // coup donné
			if (((player2->getLeftHandPos().x >= headPosLeftLimit_P1) && (player2->getLeftHandPos().x <= headPosRightLimit_P1))
				|| ((player2->getRightHandPos().x >= headPosLeftLimit_P1) && (player2->getRightHandPos().x <= headPosRightLimit_P1))) {
				if ((!player1->isOnGuard()) && (player2->getStaminaBar() >= STAMINA_LOSS_PER_PUNCH)) {
					if (player1->getLifeBar() > HP_LOSS_PER_PUNCH) {
						player1->setLifeBar(player1->getLifeBar() - HP_LOSS_PER_PUNCH);
					}
					else {
						player1->setLifeBar(0);
					}
				}
			}
			if (player2->getStaminaBar() >= STAMINA_LOSS_PER_PUNCH) {
				player2->setStaminaBar(player2->getStaminaBar() - STAMINA_LOSS_PER_PUNCH);
			}
			player2->setPunch(true);
		}

		// gestion barre stamina
		if ((player1->getLeftHandPos().y <= idleVThreshold_P1) || (player1->getRightHandPos().y <= idleVThreshold_P1)) { // perte de stamina continue au dessus du seuil de garde inférieur
			player1->setStaminaBar(player1->getStaminaBar() - staminaBarTimer_P1.getTime() * STAMINA_LOSS_NOT_IDLE_VELOCITY);
			if (player1->getStaminaBar() < 0) {
				player1->setStaminaBar(0);
			}
			staminaBarTimer_P1.start();
		}
		else { // gain de stamina au repos
			player1->setStaminaBar(player1->getStaminaBar() + staminaBarTimer_P1.getTime() * STAMINA_IDLE_GROWTH_VELOCITY);
			if (player1->getStaminaBar() > 100) {
				player1->setStaminaBar(100);
			}
			staminaBarTimer_P1.start();
		}
		if ((player2->getLeftHandPos().y <= idleVThreshold_P2) || (player2->getRightHandPos().y <= idleVThreshold_P2)) { // perte de stamina continue au dessus du seuil de garde inférieur
			player2->setStaminaBar(player2->getStaminaBar() - staminaBarTimer_P2.getTime() * STAMINA_LOSS_NOT_IDLE_VELOCITY);
			if (player2->getStaminaBar() < 0) {
				player2->setStaminaBar(0);
			}
			staminaBarTimer_P2.start();
		}
		else { // gain de stamina au repos
			player2->setStaminaBar(player2->getStaminaBar() + staminaBarTimer_P2.getTime() * STAMINA_IDLE_GROWTH_VELOCITY);
			if (player2->getStaminaBar() > 100) {
				player2->setStaminaBar(100);
			}
			staminaBarTimer_P2.start();
		}

		// gestion croissance barre de vie des deux joueurs
		if (player1->getLifeBar() > 0) {
			player1->setLifeBar(player1->getLifeBar() + lifeBarTimer_P1.getTime() * LIFE_BAR_GROWTH_VELOCITY);
			if (player1->getLifeBar() > 100) {
				player1->setLifeBar(100);
			}
			lifeBarTimer_P1.start();
		}
		if (player2->getLifeBar() > 0) {
			player2->setLifeBar(player2->getLifeBar() + lifeBarTimer_P2.getTime() * LIFE_BAR_GROWTH_VELOCITY);
			if (player2->getLifeBar() > 100) {
				player2->setLifeBar(100);
			}
			lifeBarTimer_P2.start();
		}

		// coup à nouveau possible à donner
		if ((player1->getLeftHandPos().y >= supGuardVThreshold_P1) && (player1->getRightHandPos().y >= supGuardVThreshold_P1)) {
			player1->setPunch(false);
		}
		if ((player2->getLeftHandPos().y >= supGuardVThreshold_P2) && (player2->getRightHandPos().y >= supGuardVThreshold_P2)) {
			player2->setPunch(false);
		}
	}
	else {
		// pour éviter de donner un coup directement après la sortie de pause
		player1->setPunch(true);
		player2->setPunch(true);
	}
}