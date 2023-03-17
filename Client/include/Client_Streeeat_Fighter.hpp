#pragma once

#include <iostream>
#include <string>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <mutex>

#include "Data_structs.hpp"
#include "Player.hpp"
#include "Timer.hpp"

#define TIMEOUT_VALUE 3000 // temps (ms) � partir duquel la communication avec le serveur est consid�r�e comme perdue

#define LOCAL_HOST "127.0.0.1"

unsigned short SERVER_PORT;
string SERVER_IP_ADDRESS;
SOCKADDR_IN serverAddr;
int serverAddrSize = sizeof(serverAddr);
SOCKET clientSocket;

Player* player;
Player* opponent;

timed_mutex recvDataSyncMutex; // mutex pour la gestion de l'ex�cution du thread et l'�criture des donn�es re�ues
const chrono::duration<double, ratio<1, 1000>> timeout_duration(TIMEOUT_VALUE); // type dur�e pour le timeout

// bool�ens pour g�rer la fin d'ex�cution des threads
atomic<bool> stop_flag_getAndSendPosThread = false;
atomic<bool> stop_flag_recvPlayerDataThread = false;

// fonctions ex�cut�es par les deux threads
void getAndSendPos();
void recvPlayerData();
