#pragma once

#include "Data_structs.hpp"
#include "Player.hpp"
#include "Timer.hpp"

#define PUNCH_DEPTH_SUP_LIMIT 80 // si l'on est en dessus on considère qu'un coup est donné (en % de punch_depth_range)
#define PUNCH_DEPTH_RANGE 100 // amplitude du coup max (distance centre tête - poing le plus haut) : définie lors de la calibration 
#define HP_LOSS_PER_PUNCH 10 
#define STAMINA_LOSS_PER_PUNCH 10 // perte de stamina par coup donné
#define STAMINA_LOSS_NOT_IDLE_VELOCITY 0.001 // vitesse de décroisement en position de hors-repos de la stamina barre (en %/ms)
#define STAMINA_IDLE_GROWTH_VELOCITY 0.01 // vitesse d'accroissement au repos de la stamina
#define LIFE_BAR_GROWTH_VELOCITY 0.001 // vitesse d'accroissement de la barre de vie (en %/ms)

#define HEAD_WIDTH 37
#define HEAD_HEIGHT 50
#define HEAD_POS_TOLERANCE HEAD_WIDTH/2.0 // tolerance (value+/-tol) pour le coup donné sur la tête 

// zone de garde centré sur le centre de la tête
#define HEAD_WIDTH_PROPORTION_GUARD_ZONE 1.5 
#define HEAD_HEIGHT_PROPORTION_GUARD_ZONE 1.5 

class Gameplay {
private :
	Timer<std::chrono::milliseconds> lifeBarTimer_P1;
	Timer<std::chrono::milliseconds> lifeBarTimer_P2;
	Timer<std::chrono::milliseconds> staminaBarTimer_P1;
	Timer<std::chrono::milliseconds> staminaBarTimer_P2;

	Player* player1;
	Player* player2;

	inline bool isInRectZone(const Vec2D& point, const Vec2D& rectCenter, float rectW, float rectH);

public :
	Gameplay(Player* player1, Player* player2);

	void process_data();
};



