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

#define TIMEOUT_VALUE 15000 // temps (ms) � partir duquel la communication avec les clients (ou un client) est consid�r�e comme perdue

#define LOCAL_HOST "127.0.0.1"

SOCKET serverSocket;
SOCKADDR_IN clientAddr;
int clientAddrSize = sizeof(clientAddr);

std::map<SOCKADDR_IN, Player*> playerFromAddr; // association d'un joueur � chaque client

std::mutex recvDataSyncMutex; // mutex pour la gestion de l'ex�cution du thread et l'�criture des donn�es re�ues
std::atomic<bool> dataReceived = false;

std::atomic<bool> stop_flag_recvPlayerDataThread = false;
void recvPlayerData();

bool operator<(const SOCKADDR_IN sockaddr1, const SOCKADDR_IN sockaddr2); // n�cessaire pour le conteneur map