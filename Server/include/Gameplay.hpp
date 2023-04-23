#pragma once

#include "Player.hpp"
#include "Timer.hpp"
#include "Data_structs.hpp"

#define PUNCH_DEPTH_SUP_LIMIT 80 // si l'on est en dessus on considère qu'un coup est donné
#define PUNCH_DEPTH_INF_LIMIT 20 // si l'on est en dessous on considère que le point est en position de garde
#define HEAD_POS_TOLERANCE 2 // tolerance (value+/-tol) pour le coup donné sur la tête 
#define HP_LOSS 10
#define STAMINA_LOSS_PER_PUNCH 10 // perte de stamina par coup donné
#define STAMINA_LOSS_ON_GUARD_VELOCITY 0.001 // vitesse de décroisement en position de garde de la stamina barre (en %/ms)
#define STAMINA_IDLE_GROWTH_VELOCITY 0.01 // vitesse d'accroissement au repos de la stamina
#define LIFE_BAR_GROWTH_VELOCITY 0.001 // vitesse d'accroissement de la barre de vie (en %/ms)

class Gameplay {
private :
	Timer<std::chrono::milliseconds> lifeBarTimer_P1;
	Timer<std::chrono::milliseconds> lifeBarTimer_P2;
	Timer<std::chrono::milliseconds> staminaBarTimer_P1;
	Timer<std::chrono::milliseconds> staminaBarTimer_P2;

	bool punched_P1;
	bool punched_P2;

	bool isOnGuard_P1;
	bool isOnGuard_P2;

	Player* player1;
	Player* player2;

public :
	Gameplay(Player* player1, Player* player2);

	void process_data();
};



