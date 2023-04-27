#pragma once

/******** Définitions du protocole application spécifique au jeu (encapsulé dans UDP) ********
*
* Les valeurs en float représentent des pourcentages
*
*/
#include "Vec2D.hpp"

#define MAX_NBR_LETTERS_IN_PLAYERNAME 10

#define CONNECTION_HEADING 'C'
#define POSITION_HEADING 'P'
#define START_HEADING 'S'
#define LOCAL_PLAYER_HEADING 'P'
#define OPPOSING_PLAYER_HEADING 'O' 

#define IS_PAUSED (char)0xFF
#define IS_STARTED (char)0x00

#define INITIAL_LEFTHANDPOS {45,50}
#define INITIAL_RIGHTHANDPOS {55,50}
#define INITIAL_HEADPOS {50,50}
#define INITIAL_GAMESTATE IS_PAUSED
#define INITIAL_LIFEBAR 100
#define INITIAL_STAMINABAR 100

// format des données UDP pour la connection client-serveur
typedef struct ClientToServer_Connection {
	char heading = CONNECTION_HEADING;
	char playerName[MAX_NBR_LETTERS_IN_PLAYERNAME + 1] = { 0 };
} ClientToServer_Connection_TypeDef;

// format des données UDP pour l'envoi de la position du joueur
typedef struct ClientToServer_Position {
	char heading = POSITION_HEADING;
	unsigned long long date; // permet de dater la trame pour le calcul de la latence
	Vec2D handPos[2];
	Vec2D headPos;
	char paused; // état du joueur (recalibration régulièrement nécessaire)
} ClientToServer_Position_TypeDef;

// format des données UDP pour le lancement du jeu (serveur vers client)
typedef struct ServerToClient_Start {
	char heading = START_HEADING;
	char opponentName[MAX_NBR_LETTERS_IN_PLAYERNAME + 1] = { 0 };
} ServerToClient_Start_TypeDef;


// format des données UDP pour l'envoi des données aux clients
typedef struct ServerToClient_Data {
	char heading; 	// heading : P pour les données du Joueur / O pour les données de l'adversaire 
	unsigned long long date;
	Vec2D handPos[2];
	Vec2D headPos;
	char paused;
	float lifeBar;
	float staminaBar;
} ServerToClient_Data_TypeDef;