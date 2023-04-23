#pragma once

/******** D�finitions du protocole application sp�cifique au jeu (encapusl� dans UDP) ********
*
* Les valeurs en float repr�sentent des pourcentages
*
*/

#define MAX_NBR_LETTERS_IN_PLAYERNAME 10

#define CONNECTION_HEADING 'C'
#define POSITION_HEADING 'P'
#define START_HEADING 'S'
#define LOCAL_PLAYER_HEADING 'P'
#define OPPOSING_PLAYER_HEADING 'O'

#define IS_AT_THE_LEVEL_OF_GUARD (char)0xFF // mains positionn�es au niveau de la ligne de garde (au niveau du menton)
#define IS_NOT_AT_THE_LEVEL_OF_GUARD (char)0x00 

#define INITIAL_LEFTHANDPOS 45
#define INITIAL_RIGHTHANDPOS 55
#define INITIAL_HEADPOS 50
#define INITIAL_HANDDEPTH 0
#define INITIAL_HANDSTATE IS_AT_THE_LEVEL_OF_GUARD
#define INITIAL_LIFEBAR 100
#define INITIAL_STAMINABAR 100
#define INITIAL_AFTERPUNCHDELAY 0


// format des donn�es UDP pour la connection client-serveur
typedef struct ClientToServer_Connection {
	char heading = CONNECTION_HEADING;
	char playerName[MAX_NBR_LETTERS_IN_PLAYERNAME + 1] = { 0 };
} ClientToServer_Connection_TypeDef;

// format des donn�es UDP pour l'envoi de la position du joueur
typedef struct ClientToServer_Position {
	char heading = POSITION_HEADING;
	unsigned long long date; // permet de dater la trame pour le calcul de la latence
	float handPos[2];
	float headPos;
	float handDepth[2];
	char handState;
	char paused = 0; // �tat du joueur (recalibration r�guli�rement n�cessaire)
} ClientToServer_Position_TypeDef;

// format des donn�es UDP pour le lancement du jeu (serveur vers client)
typedef struct ServerToClient_Start {
	char heading = START_HEADING;
	char opponentName[MAX_NBR_LETTERS_IN_PLAYERNAME + 1] = { 0 };
} ServerToClient_Start_TypeDef;


// format des donn�es UDP pour l'envoi des donn�es aux clients
typedef struct ServerToClient_Data {
	char heading; 	// heading : P pour les donn�es du Joueur / O pour les donn�es de l'adversaire 
	unsigned long long date;
	float handPos[2];
	float headPos;
	float handDepth[2];
	char handState;
	char paused;
	float lifeBar;
	float staminaBar;
	float afterPunchDelay; // d�lai de r�action du joueur apr�s le coup re�u
} ServerToClient_Data_TypeDef;
