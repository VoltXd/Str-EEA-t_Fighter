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

#define TIMEOUT_VALUE 3000 // temps (ms) à partir duquel la communication avec le serveur est considérée comme perdue

#define LOCAL_HOST "127.0.0.1"

unsigned short SERVER_PORT;
string SERVER_IP_ADDRESS;
SOCKADDR_IN serverAddr;
int serverAddrSize = sizeof(serverAddr);
SOCKET clientSocket;

Player* player;
Player* opponent;

timed_mutex recvDataSyncMutex; // mutex pour la gestion de l'exécution du thread et l'écriture des données reçues
const chrono::duration<double, ratio<1, 1000>> timeout_duration(TIMEOUT_VALUE); // type durée pour le timeout

// booléens pour gérer la fin d'exécution des threads
atomic<bool> stop_flag_getAndSendPosThread = false;
atomic<bool> stop_flag_recvPlayerDataThread = false;

// fonctions exécutées par les deux threads
void getAndSendPos();
void recvPlayerData();
