#pragma once

#include <iostream>
#include <string>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <chrono>

#include <thread>
#include <mutex>

#include "App.hpp"
#include "Data_structs.hpp"
#include "Player.hpp"
#include "Timer.hpp"


#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define HAND_WIDTH 50


#define TIMEOUT_VALUE 5000 // temps (ms) � partir duquel la communication avec le serveur est consid�r�e comme perdue
#define DELAY_BEFORE_AUTO_SHIFTING 250 /* temps (ms) � partir duquel le mouvement n'est plus actualis� par les donn�es re�ues 
 mais par d�placement automatique en fonction de la vitesse pr�cedente */

#define LOCAL_HOST "127.0.0.1"

unsigned short SERVER_PORT;
std::string SERVER_IP_ADDRESS;
SOCKADDR_IN serverAddr;
int serverAddrSize = sizeof(serverAddr);
SOCKET clientSocket;

Player* player;
Player* opponent;

std::mutex recvDataSyncMutex; // mutex pour la gestion de l'ex�cution du thread et l'�criture des donn�es re�ues

// bool�ens pour g�rer la fin d'ex�cution des threads
std::atomic<bool> stop_flag_getAndSendPosThread = false;
std::atomic<bool> stop_flag_recvPlayerDataThread = false;

// fonctions ex�cut�es par les deux threads
void getAndSendPos();
void recvPlayerData();

App* app;
