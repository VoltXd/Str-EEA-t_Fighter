#pragma once

#include "Player.hpp"

#define PUNCH_DEPTH_SUP_LIMIT 80 // si l'on est en dessus on considère qu'un coup est donné
#define HEAD_POS_TOLERANCE 2 // tolerance (value+/-tol) pour le coup donné sur la tête 
#define HP_LOSS 10


void gameplay(Player& player1, Player& player2);


