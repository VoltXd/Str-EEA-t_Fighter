#pragma once

#include <iostream>

#include <map>
#include <iterator>

#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")

#include <thread>
#include <mutex>
#include <chrono>

#include "Data_structs.hpp"
#include "Player.hpp"
#include "Gameplay.hpp"
#include "Timer.hpp"

#define TIMEOUT_VALUE 1000 // temps (ms) à partir duquel la communication avec les clients (ou un client) est considérée comme perdue

#define SERVER_PORT 50000

#define LOCAL_HOST "127.0.0.1"

SOCKET serverSocket;
SOCKADDR_IN clientAddr;
int clientAddrSize = sizeof(clientAddr);

std::map<SOCKADDR_IN, Player*> playerFromAddr; // association d'un joueur à chaque client

std::mutex recvDataSyncMutex; // mutex pour la gestion de l'exécution du thread et l'écriture des données reçues
std::atomic<bool> dataReceived = false;

std::atomic<bool> stop_flag_recvPlayerDataThread = false;
void recvPlayerData();

bool operator<(const SOCKADDR_IN sockaddr1, const SOCKADDR_IN sockaddr2); // nécessaire pour le conteneur map