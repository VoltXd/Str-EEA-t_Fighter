// Client_Street_Fighter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client_Streeeat_Fighter.hpp"

int main(int argc, char** argv) {
    WSADATA wsaData;

    bool isStarted = false; // vrai lorsque le jeu est lanc�

    // Initialisation Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Client: WSAStartup failed with error" << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    else {
        std::cout << "Client: The Winsock DLL status is" << wsaData.szSystemStatus << std::endl;
    }

    // Cr�ation du socket pour envoyer des donn�es au serveur (port non occup� choisi automatiquement)
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        std::cout << "Error while creating socket : " << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    else {
        std::cout << "Socket is created !" << std::endl;
    }
    
    std::cout << "Client ready" << std::endl;
    
    /* --- Demande des informations du serveur (IP + Port) --- */
    std::cout << "Server IP address : ";
    std::cin >> SERVER_IP_ADDRESS;
    std::cout << "Server port : ";
    std::cin >> SERVER_PORT;
    /* --- */

    while (1) {
        ClientToServer_Connection_TypeDef connectionData;
        ServerToClient_Start_TypeDef startData;

        /* --- Demande du nom du joueur --- */
        std::cout << "Player Name (max char. = " << MAX_NBR_LETTERS_IN_PLAYERNAME << ") : ";
        std::cin >> connectionData.playerName;
        player = new Player(connectionData.playerName); // cr�ation du joueur 
        /* --- */

        // connexion au serveur et attente de l'autre joueur
        serverAddr.sin_family = AF_INET; serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS.c_str()); serverAddr.sin_port = htons(SERVER_PORT);
        while (1) {
            /* --- Demande de connexion au serveur --- */
            std::cout << "Sending a connection request ..." << std::endl;
            sendto(clientSocket, (const char*)&connectionData, sizeof(connectionData), 0, (SOCKADDR*)&serverAddr, serverAddrSize);
            std::cout << "Wait for opponent ..." << std::endl;
            /* --- */

            /* --- V�rification de la connexion de l'autre joueur en ligne --- */
            int nbrBytesReceived = recvfrom(clientSocket, (char*)&startData, sizeof(startData),
                0, (SOCKADDR*)&serverAddr, &serverAddrSize);
            if ((nbrBytesReceived == sizeof(startData)) && (startData.heading == START_HEADING)) {
                opponent = new Player(startData.opponentName);
                std::cout << "Opponent found : " << startData.opponentName << std::endl;
                break;
            }
            else if (nbrBytesReceived == SOCKET_ERROR) {
                std::cout << "** Connection request failed **" << std::endl;
            }
            /* --- */
        }
        std::cout << "Game start" << std::endl;
        isStarted = true;
        long latency = 0;
        app = new App(SCREEN_WIDTH, SCREEN_HEIGHT);

        stop_flag_getAndSendPosThread = false;
        stop_flag_recvPlayerDataThread = false;
        std::thread sendPosThread(getAndSendPos); // lancement du thread qui r�cup�re la position et l'envoit r�guli�rement au serveur
        std::thread recvPlayerDataThread(recvPlayerData); // lancement du thread qui r�cup�re r�guli�rement les donn�es re�ues du serveur

        player->dataAreReceived();
        opponent->dataAreReceived();
        while (isStarted) {
            /* --- stockage des donn�es + gestion pertes de communication --- */
            // gestion perte de communication avec le serveur
            recvDataSyncMutex.lock();
            if (player->checkTime() >= TIMEOUT_VALUE) {
                std::cout << "No data received for the local player " << player->getName() << " - Timeout reached" << std::endl;
                isStarted = false;
            }
            if (opponent->checkTime() >= TIMEOUT_VALUE) {
                std::cout << "No data received for the opponent " << opponent->getName() << " - Timeout reached" << std::endl;
                isStarted = false;
            }

            if (player->checkTime() <= DELAY_BEFORE_AUTO_SHIFTING) {
                // stockage des derni�res donn�es correctes re�ues
                player->pullLastReceivedData();
            }
            else {
                // d�placement automatique tant qu'aucune autre donn�e est re�ue
                player->updatePosAutoShifting((float)HAND_WIDTH/SCREEN_WIDTH, (float)HAND_WIDTH/SCREEN_HEIGHT);
            }
            if (opponent->checkTime() <= DELAY_BEFORE_AUTO_SHIFTING) {
                // stockage des derni�res donn�es correctes re�ues
                opponent->pullLastReceivedData();
            }
            else {
                // d�placement automatique tant qu'aucune autre donn�e est re�ue
                opponent->updatePosAutoShifting((float)HAND_WIDTH/SCREEN_WIDTH, (float)HAND_WIDTH/SCREEN_HEIGHT);
            }
            latency = clock() - player->getLastReceivedData().date;
            //std::cout << player->getLastReceivedData().date << std::endl;
            //std::cout << "leftHandPosOpp(recv) : " << opponent->getLeftHandPos() << std::endl;

            recvDataSyncMutex.unlock();
            /* --- */

            /* --- Affichage graphique des joueurs --- */
            if (app->exit()) isStarted = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(5));
            app->drawRect(player->getLeftHandPos(), player->getRightHandPos(), HAND_WIDTH, {255,255,255,255});
            std::cout << latency << std::endl;
            /* --- */
        }
        std::cout << "Game stop - All players are automatically disconnected" << std::endl;

        /* --- fermeture des threads --- */
        // arr�t du thread de l'envoi des positions (fin de la boucle en cours)
        stop_flag_getAndSendPosThread = true;
        sendPosThread.join();
        delete app;

        // arr�t du thread de r�ception
        SOCKADDR_IN localhostAddr;
        int localhostAddrLength = sizeof(localhostAddr);
        getsockname(clientSocket, (SOCKADDR*)&localhostAddr, &localhostAddrLength); // r�cup�ration du port associ� au socket
        localhostAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST); // adresse local
        sendto(clientSocket, "0", 1, 0, (SOCKADDR*)&localhostAddr, localhostAddrLength); // envoi d'un datagramme quelconque
        // en local host pour d�bloquer la fonction recv du thread une fois
        stop_flag_recvPlayerDataThread = true;
        recvPlayerDataThread.join();
        /* --- */

        delete player;
        delete opponent;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}

void getAndSendPos() {
    // simulation de la position (balayage gauche droite) + envoi de la position au serveur
    ClientToServer_Position_TypeDef posDataToSend;
    posDataToSend.handPos[0] = 0, posDataToSend.handPos[1] = 100, posDataToSend.headPos = 50;
    posDataToSend.punchDepth = 0; posDataToSend.handState = 1;

    int direction = 1;
    while (!stop_flag_getAndSendPosThread) {
        posDataToSend.date = clock();
        posDataToSend.handPos[0] += direction * 0.5; posDataToSend.handPos[1] -= direction * 0.5;
        if ((posDataToSend.handPos[0] >= 100) || (posDataToSend.handPos[0] <= 0) || (posDataToSend.handPos[1] >= 100) || (posDataToSend.handPos[1] <= 0)) {
            direction = -direction;
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(50));
        //std::cout << "leftHandPos(sent) : " << posDataToSend.handPos[0] << std::endl;
        sendto(clientSocket, (const char*)&posDataToSend, sizeof(posDataToSend), 0, (const sockaddr*)&serverAddr, serverAddrSize);
    }
}

void recvPlayerData() {
    ServerToClient_Data_TypeDef receiptBuffer; // buffer de r�ception des donn�es d'un joueur

    while (!stop_flag_recvPlayerDataThread) {
        int nbrBytesReceived = recvfrom(clientSocket, (char*)&receiptBuffer, sizeof(receiptBuffer),
            0, (SOCKADDR*)&serverAddr, &serverAddrSize);

        recvDataSyncMutex.lock(); // attente de la lib�ration du mutex et lock du mutex

        // si les donn�es sont formatt�es correctement
        if (nbrBytesReceived == sizeof(receiptBuffer)) {
            switch (receiptBuffer.heading) {
            case LOCAL_PLAYER_HEADING:
                player->setPrevReceivedData(player->getLastReceivedData());
                player->setLastReceivedData(receiptBuffer);
                player->setDelayBtw2LastData(player->checkTime());
                player->dataAreReceived();
                break;
            case OPPOSING_PLAYER_HEADING:
                opponent->setPrevReceivedData(opponent->getLastReceivedData());
                opponent->setLastReceivedData(receiptBuffer);
                opponent->setDelayBtw2LastData(opponent->checkTime());
                opponent->dataAreReceived();
                break;
            default: break;
            }
        }

        recvDataSyncMutex.unlock();
        //std::this_thread::sleep_for(std::chrono::milliseconds(5)); // permet aux autres thread de laisser le temps d'utiliser la variable des donn�es r�cup�r�es
    }
}

// Run program: Ctrl + F5 or Debug > Start Without Debugging menu
// Debug program: F5 or Debug > Start Debugging menu

// Tips for Getting Started: 
//   1. Use the Solution Explorer window to add/manage files
//   2. Use the Team Explorer window to connect to source control
//   3. Use the Output window to see build output and other messages
//   4. Use the Error List window to view errors
//   5. Go to Project > Add New Item to create new code files, or Project > Add Existing Item to add existing code files to the project
//   6. In the future, to open this project again, go to File > Open > Project and select the .sln file
