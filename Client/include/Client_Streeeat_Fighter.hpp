#pragma once

// partie communication client-serveur 
#include <iostream>
#include <string>
#include <WinSock2.h>
#pragma comment(lib, "ws2_32.lib")
#include <chrono>
#include <thread>
#include <mutex>
#include "SDL.h"

#include "App.hpp"
#include "Data_structs.hpp"
#include "PlayerOnline.hpp"
#include "Timer.hpp"

// partie traitement video
#include "WebcamManager.hpp"

#define THRESHOLD_RATIO 0.1 // pourcentage des valeurs de rgb de calibration des poings (seuillage)

#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 720

#define TIMEOUT_VALUE 15000 // temps (ms) � partir duquel la communication avec le serveur est consid�r�e comme perdue

#define LOCAL_HOST "127.0.0.1"

// Fonctions de configuration du socket
int initClientSocket(const std::string& ip, unsigned short port);
void closeClientSocket();
void connectToSFServer();
void startClientCommunication();
bool updateClientData();
void renderClient(SDL_Renderer* renderer);

// fonctions ex�cut�es par les deux threads
void getAndSendPos();
void recvPlayerData();

