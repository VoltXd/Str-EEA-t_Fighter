// Client_Street_Fighter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client_Streeeat_Fighter.hpp"

int main()	{
    WSADATA wsaData;

    bool isStarted = false; // vrai lorsque le jeu est lancé

    // Initialisation Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        cout << "Client: WSAStartup failed with error" << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "Client: The Winsock DLL status is" << wsaData.szSystemStatus << endl;
    }

    // Création du socket pour envoyer des données au serveur (port non occupé choisi automatiquement)
    clientSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (clientSocket == INVALID_SOCKET) {
        cout << "Error while creating socket : " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "Socket is created !" << endl;
    }
    
    cout << "Client ready" << endl;
    
    /* --- Demande des informations du serveur (IP + Port) --- */
    cout << "Server IP address : ";
    cin >> SERVER_IP_ADDRESS;
    cout << "Server port : ";
    cin >> SERVER_PORT;

    /* --- */

    while (1) {
        ClientToServer_Connection_TypeDef connectionData;
        ServerToClient_Start_TypeDef startData;

        /* --- Demande du nom du joueur --- */
        cout << "Player Name (max char. = " << MAX_NBR_LETTERS_IN_PLAYERNAME << ") : ";
        cin >> connectionData.playerName;
        player = new Player(connectionData.playerName); // création du joueur 
        /* --- */

        // connexion au serveur et attente de l'autre joueur
        serverAddr.sin_family = AF_INET; serverAddr.sin_addr.s_addr = inet_addr(SERVER_IP_ADDRESS.c_str()); serverAddr.sin_port = htons(SERVER_PORT);
        while (1) {
            /* --- Demande de connexion au serveur --- */
            cout << "Sending a connection request ..." << endl;
            sendto(clientSocket, (const char*)&connectionData, sizeof(connectionData), 0, (SOCKADDR*)&serverAddr, serverAddrSize);
            cout << "Wait for opponent ..." << endl;
            /* --- */

            /* --- Vérification de la connexion de l'autre joueur en ligne --- */
            int nbrBytesReceived = recvfrom(clientSocket, (char*)&startData, sizeof(startData),
                0, (SOCKADDR*)&serverAddr, &serverAddrSize);
            if ((nbrBytesReceived == sizeof(startData)) && (startData.heading == START_HEADING)) {
                opponent = new Player(startData.opponentName);
                cout << "Opponent found : " << startData.opponentName << endl;
                break;
            }
            else if (nbrBytesReceived == SOCKET_ERROR) {
                cout << "** Connection request failed **" << endl;
            }
            /* --- */
        }
        cout << "Game start" << endl;
        isStarted = true;

        stop_flag_getAndSendPosThread = false;
        stop_flag_recvPlayerDataThread = false;
        thread sendPosThread(getAndSendPos); // lancement du thread qui récupère la position et l'envoit régulièrement au serveur
        thread recvPlayerDataThread(recvPlayerData); // lancement du thread qui récupère régulièrement les données reçues du serveur

        player->dataAreReceived();
        opponent->dataAreReceived();
        // récupération des données centralisées sur le serveur
        while (isStarted) {
            /* --- stockage des données + gestion pertes de communication --- */
            // gestion perte de communication avec le serveur
            recvDataSyncMutex.lock();
            if (player->checkTime() >= TIMEOUT_VALUE) {
                cout << "No data received for the local player " << player->getName() << " - Timeout reached" << endl;
                isStarted = false;
            }
            if (opponent->checkTime() >= TIMEOUT_VALUE) {
                cout << "No data received for the opponent " << opponent->getName() << " - Timeout reached" << endl;
                isStarted = false;
            }

            if (player->checkTime() <= DELAY_BEFORE_AUTO_SHIFTING) {
                // stockage des dernières données correctes reçues
                player->pullLastReceivedData();
            }
            else {
                // déplacement automatique tant qu'aucune autre donnée est reçue
                player->updatePosAutoShifting(0, 100);
            }
            if (opponent->checkTime() <= DELAY_BEFORE_AUTO_SHIFTING) {
                // stockage des dernières données correctes reçues
                opponent->pullLastReceivedData();
            }
            else {
                // déplacement automatique tant qu'aucune autre donnée est reçue
                opponent->updatePosAutoShifting(0, 100);
            }
            cout << "leftHandPosOpp(recv) : " << opponent->getLeftHandPos() << endl;

            recvDataSyncMutex.unlock();
            /* --- */

            /* --- Affichage graphique des joueurs --- */
            Sleep(200);
            /* --- */
        }
        cout << "Game stop - All players are automatically disconnected" << endl;

        /* --- fermeture des threads --- */
        // arrêt du thread de l'envoi des positions (fin de la boucle en cours)
        stop_flag_getAndSendPosThread = true;
        sendPosThread.join();

        // arrêt du thread de réception
        SOCKADDR_IN localhostAddr;
        int localhostAddrLength = sizeof(localhostAddr);
        getsockname(clientSocket, (SOCKADDR*)&localhostAddr, &localhostAddrLength); // récupération du port associé au socket
        localhostAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST); // adresse local
        sendto(clientSocket, "0", 1, 0, (SOCKADDR*)&localhostAddr, localhostAddrLength); // envoi d'un datagramme quelconque
        // en local host pour débloquer la fonction recv du thread une fois
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

    posDataToSend.handPos[0] = 0, posDataToSend.handPos[1] = 100, posDataToSend.headPos = 50, posDataToSend.punchDepth = 0; posDataToSend.handState = 1;
    int direction = 1;
    while (!stop_flag_getAndSendPosThread) {

        posDataToSend.handPos[0] += direction * 0.01; posDataToSend.handPos[1] -= direction * 0.01;
        if ((posDataToSend.handPos[0] >= 100) || (posDataToSend.handPos[0] <= 0) || (posDataToSend.handPos[1] >= 100) || (posDataToSend.handPos[1] <= 0)) {
            direction = -direction;
        }

        Sleep(30);
        cout << "leftHandPos(sent) : " << posDataToSend.handPos[0] << endl;
        sendto(clientSocket, (const char*)&posDataToSend, sizeof(posDataToSend), 0, (const sockaddr*)&serverAddr, serverAddrSize);
    }
}

void recvPlayerData() {
    ServerToClient_Data_TypeDef receiptBuffer; // buffer de réception des données d'un joueur

    while (!stop_flag_recvPlayerDataThread) {
        int nbrBytesReceived = recvfrom(clientSocket, (char*)&receiptBuffer, sizeof(receiptBuffer),
            0, (SOCKADDR*)&serverAddr, &serverAddrSize);

        recvDataSyncMutex.lock(); // attente de la libération du mutex et lock du mutex

        // si les données sont formattées correctement
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
        this_thread::sleep_for(chrono::milliseconds(10)); // permet aux autres thread de laisser le temps d'utiliser la variable des données récupérées
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
