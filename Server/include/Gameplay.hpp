#pragma once

#include "Player.hpp"

#define PUNCH_DEPTH_SUP_LIMIT 80 // si l'on est en dessus on consid�re qu'un coup est donn�
#define HEAD_POS_TOLERANCE 2 // tolerance (value+/-tol) pour le coup donn� sur la t�te 
#define HP_LOSS 10


void gameplay(Player& player1, Player& player2);


