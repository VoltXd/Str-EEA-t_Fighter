// Client_Serveur_Street_Fighter.cpp : This file contains the 'main' function. Program execution begins and ends there.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Server_Streeeat_Fighter.hpp"

int main()  {
    WSADATA wsaData;
    
    SOCKADDR_IN initServerSocket;
    initServerSocket.sin_family = AF_INET; initServerSocket.sin_port = htons(SERVER_PORT); initServerSocket.sin_addr.s_addr = INADDR_ANY;

    bool isStarted = false; // vrai lorsque le jeu est lancé

    // Initialisation Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)  {
        std::cout << "Server: WSAStartup failed with error" << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Server: The Winsock DLL status is" << wsaData.szSystemStatus << std::endl;

    // Création du socket pour recevoir les datagrammes
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        std::cout << "Error while creating socket : " << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    std::cout << "Socket is created !" << std::endl;
    
    /* --- Liaison du socket avec l'extérieur et le port ouvert côté serveur  --- */
    if (bind(serverSocket, (SOCKADDR*)&initServerSocket, sizeof(initServerSocket)) == SOCKET_ERROR)  {
        std::cout << "Error while binding socket : " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }
    else {
        std::cout << "Socket binding is completed with success" << std::endl;
    }

    std::cout << "Server ready - Listening on port " << ntohs(initServerSocket.sin_port) << " ..." << std::endl;
    /* --- */

    while (1) {
        Player player[2]; // les deux joueurs 

        /* --- phase de connexion pour les deux clients et association des joueurs aux clients --- */
        std::cout << "Searching for players ..." << std::endl;
        ClientToServer_Connection_TypeDef connectionData;
        while (playerFromAddr.size() < 2) {
            int nbrBytesReceived = recvfrom(serverSocket, (char*)&connectionData, sizeof(connectionData),
                0, (SOCKADDR*)&clientAddr, &clientAddrSize);

            if (nbrBytesReceived > 0) {
                //cout << "UDP data received from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;

                // si on reçoit une demande de connexion 
                if ((nbrBytesReceived == sizeof(connectionData)) && (connectionData.heading == CONNECTION_HEADING)) {
                    // ajout d'un joueur seulement s'il n'est pas déjà ajouté
                    int size = playerFromAddr.size();
                    playerFromAddr[clientAddr] = player + size;
                    playerFromAddr[clientAddr]->setName(connectionData.playerName);
                    playerFromAddr[clientAddr]->setAddr(clientAddr);
                    std::cout << "Player " << playerFromAddr.size() << " connected - "
                         << playerFromAddr[clientAddr]->getName() << " - " 
                         << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << std::endl;
                }
            }
        }
        /* --- */

        /* --- On informe les joueurs que le jeu peut démarrer --- */
        // envoi au joueur 0 le nom du joueur 1 et le départ du jeu
        player[0].sendStartDatagram(serverSocket, player[1].getName());

        // envoi au joueur 1 le nom du joueur 0 et le départ du jeu
        player[1].sendStartDatagram(serverSocket, player[0].getName());
        
        std::cout << "Players are ready - Game start" << std::endl;
        Gameplay gameplay(&player[0], &player[1]);
        isStarted = true;
        /* --- */

        stop_flag_recvPlayerDataThread = false;
        std::thread recvPlayerDataThread(recvPlayerData); // lancement du thread pour écouter les datagrammes reçus des clients

        player[0].dataAreReceived();
        player[1].dataAreReceived();
        while (isStarted) {

            /* --- stockage des données + gestion pertes de communication  + vérification fin du jeu --- */
            recvDataSyncMutex.lock();
            // gestion de la perte de communication éventuelle avec les joueurs
            if (player[0].checkTime() >= TIMEOUT_VALUE) {
                std::cout << "No data received from player " << player[0].getName() << " - Timeout reached" << std::endl;
                isStarted = false;
            }
            if (player[1].checkTime() >= TIMEOUT_VALUE) {
                std::cout << "No data received from player " << player[1].getName() << " - Timeout reached" << std::endl;
                isStarted = false;
            }
            // ici soit le mutex est accaparé soit le thread est bloqué par recv (aucune réception et on coupe la connexion)
           
            // stockage des dernières données correctes reçues
            player[0].pullLastReceivedData();
            player[1].pullLastReceivedData();
           
            recvDataSyncMutex.unlock();
            /* --- */
            
            /* --- Gameplay --- */
            //std::this_thread::sleep_for(std::chrono::milliseconds(20));
            gameplay.process_data();
            /* --- */

            /* --- Envoi des données mise à jour aux joueurs --- */
            player[0].pushCurrentPlayerData(serverSocket, player[1].getAddr());
            player[1].pushCurrentPlayerData(serverSocket, player[0].getAddr());
            /* --- */
        }
        std::cout << "Game stop - All players are automatically disconnected" << std::endl;

        /* --- fermeture des threads --- */
        // arrêt du thread de réception
        stop_flag_recvPlayerDataThread = true;
        SOCKADDR_IN localhostAddr;
        localhostAddr.sin_family = AF_INET; localhostAddr.sin_port = htons(SERVER_PORT); localhostAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
        sendto(serverSocket, "0", 1, 0, (SOCKADDR*)&localhostAddr, sizeof(localhostAddr)); // envoi d'un datagramme quelconque
        // en local host pour débloquer la fonction recv du thread une fois 
        recvPlayerDataThread.join();

        playerFromAddr.clear(); // suppression des joueurs
        /* --- */
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void recvPlayerData() {
    while (!stop_flag_recvPlayerDataThread.load()) {
        ClientToServer_Position_TypeDef receiptBuffer; // buffer de réception de la position

        int nbrBytesReceived = recvfrom(serverSocket, (char*)&receiptBuffer, sizeof(receiptBuffer),
            0, (SOCKADDR*)&clientAddr, &clientAddrSize);

        recvDataSyncMutex.lock(); // attente de la libération du mutex et lock du mutex

        // si il s'agit d'un des deux joueurs qui a envoyé des données
        if (playerFromAddr.count(clientAddr)) {
            // si il s'agit bien des données de position formattées correctement
            if ((nbrBytesReceived == sizeof(receiptBuffer)) && (receiptBuffer.heading == POSITION_HEADING) && (playerFromAddr.count(clientAddr) == 1)) {
                playerFromAddr[clientAddr]->setLastReceivedData(receiptBuffer);
                playerFromAddr[clientAddr]->dataAreReceived();

            }
        }

        recvDataSyncMutex.unlock();
        //std::this_thread::sleep_for(std::chrono::milliseconds(5)); // permet au programme principal de lui laisser le temps d'utiliser la variable des données récupérées
    }
}

inline bool operator<(const SOCKADDR_IN sockaddr1, const SOCKADDR_IN sockaddr2) {
    // comparaison arbitraire
    return (sockaddr1.sin_port < sockaddr2.sin_port); 
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
