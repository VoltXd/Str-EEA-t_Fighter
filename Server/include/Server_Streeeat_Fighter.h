#pragma once

#include <iostream>

#include <map>
#include <iterator>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <mutex>

#include <ctime>

#include "Data_structs.h"
#include "Player.h"
#include "Gameplay.h"
#include "Timer.h"

#define TIMEOUT_VALUE 3000 // temps (ms) à partir duquel la communication avec les clients (ou un client) est considérée comme perdue

using namespace std;

#define SERVER_PORT 50000

#define LOCAL_HOST "127.0.0.1"

SOCKET serverSocket;
SOCKADDR_IN clientAddr;
int clientAddrSize = sizeof(clientAddr);

map<SOCKADDR_IN, Player*> playerFromAddr; // association d'un joueur à chaque client

timed_mutex recvDataSyncMutex; // mutex pour la gestion de l'exécution du thread et l'écriture des données reçues
const chrono::duration<double, ratio<1, 1000>> timeout_duration(TIMEOUT_VALUE); // type durée pour le timeout


atomic<bool> stop_flag_recvPlayerDataThread = false;
void recvPlayerData();

bool operator<(const SOCKADDR_IN sockaddr1, const SOCKADDR_IN sockaddr2); // nécessaire pour le conteneur map