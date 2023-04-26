#pragma once

// partie communication client-serveur 
#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <chrono>
#include <thread>
#include <mutex>

#include "App.hpp"
#include "Data_structs.hpp"
#include "PlayerOnline.hpp"
#include "Timer.hpp"

// partie traitement video
#include "WebcamManager.hpp"

#define THRESHOLD_RATIO 0.1 // pourcentage des valeurs de rgb de calibration des poings (seuillage)

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define HAND_WIDTH 20
#define HAND_HEIGHT 20
#define HEAD_WIDTH 40
#define HEAD_HEIGHT 60

#define TIMEOUT_VALUE 7000 // temps (ms) � partir duquel la communication avec le serveur est consid�r�e comme perdue

#define LOCAL_HOST "127.0.0.1"

unsigned short SERVER_PORT;
std::string SERVER_IP_ADDRESS;
SOCKADDR_IN serverAddr;
int serverAddrSize = sizeof(serverAddr);
SOCKET clientSocket;

PlayerOnline* player;
PlayerOnline* opponent;

std::mutex recvDataSyncMutex; // mutex pour la gestion de l'ex�cution du thread et l'�criture des donn�es re�ues

// bool�ens pour g�rer la fin d'ex�cution des threads
std::atomic<bool> stop_flag_getAndSendPosThread = false;
std::atomic<bool> stop_flag_recvPlayerDataThread = false;

// fonctions ex�cut�es par les deux threads
void getAndSendPos();
void recvPlayerData();

App* app;
