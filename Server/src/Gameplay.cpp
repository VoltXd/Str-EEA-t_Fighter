#include "Gameplay.hpp"

void gameplay(Player& player1, Player& player2) {
	if ((!player1.getPaused()) && (!player2.getPaused())) {
		if (player1.getLeftHandDepth() >= PUNCH_DEPTH_SUP_LIMIT) {
			if ((player1.getLeftHandPos() >= player2.getHeadPos() - HEAD_POS_TOLERANCE) && (player1.getLeftHandPos() <= player2.getHeadPos() + HEAD_POS_TOLERANCE)) {
				if (player2.getLifeBar() > HP_LOSS) {
					player2.setLifeBar(player2.getLifeBar() - HP_LOSS);
				}
				else {
					// fin du jeu player2 perdu
				}
			}
		}
	}
}