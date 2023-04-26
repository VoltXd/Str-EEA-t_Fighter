// Client_Street_Fighter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client_Streeeat_Fighter.hpp"

int main_(int argc, char** argv) {
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
        ClientToServer_Connection connectionData;
        ServerToClient_Start startData = { 0, {0} };

        /* --- Demande du nom du joueur --- */
        std::cout << "Player Name (max char. = " << MAX_NBR_LETTERS_IN_PLAYERNAME << ") : ";
        std::cin >> connectionData.playerName;
        player = new PlayerOnline(connectionData.playerName); // cr�ation du joueur 
        /* --- */

        // connexion au serveur et attente de l'autre joueur
        serverAddr.sin_family = AF_INET; serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS.c_str()); serverAddr.sin_port = htons(SERVER_PORT);
        
        /* --- Demande de connexion au serveur --- */
        std::cout << "Sending a connection request ..." << std::endl;
        sendto(clientSocket, (const char*)&connectionData, sizeof(connectionData), 0, (SOCKADDR*)&serverAddr, serverAddrSize);
        std::cout << "Wait for opponent ..." << std::endl;
        /* --- */

        /* --- Attente de la connexion de l'autre joueur en ligne --- */
        int nbrBytesReceived = 0;
        while ((nbrBytesReceived != sizeof(startData)) || (startData.heading != START_HEADING)) {
            nbrBytesReceived = recvfrom(clientSocket, (char*)&startData, sizeof(startData),
                0, (SOCKADDR*)&serverAddr, &serverAddrSize);
        }
        opponent = new PlayerOnline(startData.opponentName);
        std::cout << "Opponent found : " << startData.opponentName << std::endl;
        /* --- */

        std::cout << "Game start" << std::endl;
        isStarted = true;
        // app = new App(SCREEN_WIDTH, SCREEN_HEIGHT);

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

            player->pullLastReceivedData();
            player->updatePosAutoShifting((float)HEAD_WIDTH / SCREEN_WIDTH, (float)HEAD_HEIGHT / SCREEN_HEIGHT, 
                (float)HAND_WIDTH / SCREEN_WIDTH, (float)HAND_HEIGHT / SCREEN_HEIGHT);
            opponent->pullLastReceivedData();
            opponent->updatePosAutoShifting((float)HEAD_WIDTH / SCREEN_WIDTH, (float)HEAD_HEIGHT / SCREEN_HEIGHT,
                (float)HAND_WIDTH / SCREEN_WIDTH, (float)HAND_HEIGHT / SCREEN_HEIGHT);

            recvDataSyncMutex.unlock();
            /* --- */

            /* --- Affichage graphique des joueurs --- */
            // if (app->exit()) isStarted = false;
            if (!player->isPaused() && !opponent->isPaused()) {
                // app->renderClear();
                // // joueur local
                // app->drawRect(player->getLeftHandPos().x, player->getLeftHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,255,255,255 });
                // app->drawRect(player->getRightHandPos().x, player->getRightHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,255,255,255 });
                // app->drawRect(player->getHeadPos().x, player->getHeadPos().y, HEAD_WIDTH, HEAD_HEIGHT, { 255,255,255,255 });
                // // adversaire
                // app->drawRect(opponent->getLeftHandPos().x, opponent->getLeftHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,0,0,255 });
                // app->drawRect(opponent->getRightHandPos().x, opponent->getRightHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,0,0,255 });
                // app->drawRect(opponent->getHeadPos().x, opponent->getHeadPos().y, HEAD_WIDTH, HEAD_HEIGHT, { 255,0,0,255 });
                // app->renderPresent();
            }
            /* --- */
        }
        delete app;
        std::cout << "Game stop - All players are automatically disconnected" << std::endl;

        /* --- fermeture des threads --- */
        // arr�t du thread de l'envoi des positions (fin de la boucle en cours)
        stop_flag_getAndSendPosThread = true;
        sendPosThread.join();

        // arr�t du thread de r�ception
        stop_flag_recvPlayerDataThread = true;
        SOCKADDR_IN localhostAddr;
        int localhostAddrLength = sizeof(localhostAddr);
        getsockname(clientSocket, (SOCKADDR*)&localhostAddr, &localhostAddrLength); // r�cup�ration du port associ� au socket
        localhostAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST); // adresse local
        sendto(clientSocket, "0", 1, 0, (SOCKADDR*)&localhostAddr, localhostAddrLength); // envoi d'un datagramme quelconque
        // en local host pour d�bloquer la fonction recv du thread une fois
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
    // Trame de position pour l'envoi au serveur
    ClientToServer_Position posDataToSend;
    posDataToSend.handPos[0] = Vec2D(45, 50), posDataToSend.handPos[1] = Vec2D(55, 50), posDataToSend.headPos = Vec2D(50, 40);
    posDataToSend.paused = INITIAL_GAMESTATE;

    /* --- Video processing init --- */
    WebcamManager cam(NULL);
    if (!cam.isCameraOpened()) {
        std::cout << "Unable to open camera" << std::endl;
    }
    /* --- */

    //float direction = 1;
    while (!stop_flag_getAndSendPosThread.load()) {
        //std::this_thread::sleep_for(std::chrono::milliseconds(30));

        /*posDataToSend.handPos[0] += Vec2D(direction * 0.05, direction * 0.05); posDataToSend.handPos[1] -= Vec2D(direction * 0.05, direction * 0.05);
        if ((posDataToSend.handPos[0] >= 100) || (posDataToSend.handPos[0] <= 0) || (posDataToSend.handPos[1] >= 100) || (posDataToSend.handPos[1] <= 0)) {
            direction = -direction;
        }*/

        /* --- image processing --- */
        // Calibration starts when pressing the ENTER key
        if (cv::waitKey(1) == 13) {
            cam.startCalibration();
        }
        cam.nextAction();
        cam.CV_render();
        /* --- */

        /* --- mise en forme des donn�es dans la trame d'envoi --- */
        posDataToSend.handPos[0].x = cam.getLeftHandX();
        posDataToSend.handPos[0].y = cam.getLeftHandY();
        posDataToSend.handPos[1].x = cam.getRightHandX();
        posDataToSend.handPos[1].y = cam.getRightHandY();

        posDataToSend.headPos.x = cam.getHeadX();
        posDataToSend.headPos.y = cam.getHeadY();
        posDataToSend.paused = cam.isCalibrating() ? IS_PAUSED : IS_STARTED;
        /* --- */

        posDataToSend.date = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        sendto(clientSocket, (const char*)&posDataToSend, sizeof(posDataToSend), 0, (const sockaddr*)&serverAddr, serverAddrSize);
    }
}

void recvPlayerData() {
    unsigned long long latency = 0;

    while (!stop_flag_recvPlayerDataThread.load()) {
        ServerToClient_Data receiptBuffer; // buffer de r�ception des donn�es d'un joueur

        int nbrBytesReceived = recvfrom(clientSocket, (char*)&receiptBuffer, sizeof(receiptBuffer),
            0, (SOCKADDR*)&serverAddr, &serverAddrSize);

        // si les donn�es sont formatt�es correctement
        if (nbrBytesReceived == sizeof(receiptBuffer)) {
            switch (receiptBuffer.heading) {
            case LOCAL_PLAYER_HEADING:
                recvDataSyncMutex.lock(); // attente de la lib�ration du mutex et lock du mutex
                player->setPrevReceivedData(player->getLastReceivedData());
                player->setLastReceivedData(receiptBuffer);
                
                // si les derni�res donn�es re�ues sont des nouvelles donn�es et non pas les m�mes (donn�es de position uniquement)
                if (player->getLastReceivedData().date != player->getPrevReceivedData().date) {
                    // calcul de la latence
                    std::chrono::microseconds date(player->getLastReceivedData().date);
                    latency = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()) - date).count();
                    std::cout << "Latency : " << latency << std::endl;

                    // mise � jour des param�tres pour le d�placement
                    player->setAutoShiftingParameters();
                }

                player->dataAreReceived();
                recvDataSyncMutex.unlock();
                break;
            case OPPOSING_PLAYER_HEADING:
                recvDataSyncMutex.lock(); // attente de la lib�ration du mutex et lock du mutex
                opponent->setPrevReceivedData(opponent->getLastReceivedData());
                opponent->setLastReceivedData(receiptBuffer);

                if (opponent->getLastReceivedData().date != opponent->getPrevReceivedData().date) {
                    // mise � jour des param�tres pour le d�placement
                    opponent->setAutoShiftingParameters();
                }
                
                opponent->dataAreReceived();
                recvDataSyncMutex.unlock();
                break;
            default: break;
            }
        }
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
