// Client_Serveur_Street_Fighter.cpp : This file contains the 'main' function. Program execution begins and ends there.

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Server_Streeeat_Fighter.hpp"

int main()  {
    WSADATA wsaData;
    
    SOCKADDR_IN initServerSocket;
    initServerSocket.sin_family = AF_INET; initServerSocket.sin_port = htons(SERVER_PORT); initServerSocket.sin_addr.s_addr = INADDR_ANY;

    ClientToServer_Connection_TypeDef connectionData;

    bool isStarted = false; // vrai lorsque le jeu est lanc�

    // Initialisation Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)  {
        cout << "Server: WSAStartup failed with error" << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "Server: The Winsock DLL status is" << wsaData.szSystemStatus << endl;
    }

    // Cr�ation du socket pour recevoir les datagrammes
    serverSocket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (serverSocket == INVALID_SOCKET) {
        cout << "Error while creating socket : " << WSAGetLastError() << endl;
        return EXIT_FAILURE;
    }
    else {
        cout << "Socket is created !" << endl;
    }

    /* --- Liaison du socket avec l'ext�rieur et le port ouvert c�t� serveur  --- */
    if (bind(serverSocket, (SOCKADDR*)&initServerSocket, sizeof(initServerSocket)) == SOCKET_ERROR)  {
        cout << "Error while binding socket : " << WSAGetLastError() << endl;
        closesocket(serverSocket);
        WSACleanup();
        return EXIT_FAILURE;
    }
    else {
        cout << "Socket binding is completed with success" << endl;
    }

    cout << "Server ready - Listening on port " << ntohs(initServerSocket.sin_port) << " ..." << endl;
    /* --- */

    while (1) {
        Player player[2]; // les deux joueurs 

        /* --- phase de connexion pour les deux clients et association des joueurs aux clients --- */
        cout << "Searching for players ..." << endl;
        while (playerFromAddr.size() < 2) {
            int nbrBytesReceived = recvfrom(serverSocket, (char*)&connectionData, sizeof(connectionData),
                0, (SOCKADDR*)&clientAddr, &clientAddrSize);

            if (nbrBytesReceived > 0) {
                //cout << "UDP data received from " << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;

                // si on re�oit une demande de connexion 
                if ((nbrBytesReceived == sizeof(connectionData)) && (connectionData.heading == CONNECTION_HEADING)) {
                    // ajout d'un joueur seulement s'il n'est pas d�j� ajout�
                    playerFromAddr[clientAddr] = player + playerFromAddr.size();
                    playerFromAddr[clientAddr]->setName(connectionData.playerName);
                    playerFromAddr[clientAddr]->setAddr(clientAddr);
                    cout << "Player " << playerFromAddr.size() << " connected - " 
                         << playerFromAddr[clientAddr]->getName() << " - " 
                         << inet_ntoa(clientAddr.sin_addr) << ":" << ntohs(clientAddr.sin_port) << endl;
                }
            }
        }
        /* --- */

        /* --- On informe les joueurs que le jeu peut d�marrer --- */
        // envoi au joueur 0 le nom du joueur 1 et le d�part du jeu
        player[0].sendStartDatagram(serverSocket, player[1].getName());

        // envoi au joueur 1 le nom du joueur 0 et le d�part du jeu
        player[1].sendStartDatagram(serverSocket, player[0].getName());
        
        cout << "Players are ready - Game start" << endl;
        isStarted = true;
        /* --- */

        stop_flag_recvPlayerDataThread = false;
        thread recvPlayerDataThread(recvPlayerData); // lancement du thread pour �couter les datagrammes re�us des clients

        player[0].dataAreReceived();
        player[1].dataAreReceived();
        while (isStarted) {

            /* --- stockage des donn�es + gestion pertes de communication  + v�rification fin du jeu --- */
            // gestion perte de communication g�n�rale (recv bloqu� dans le thread assez longtemps)
            if (!recvDataSyncMutex.try_lock_for(timeout_duration)) { 
                cout << "No data received from all players - Timeout reached" << endl;
                isStarted = false;
            }
            // gestion de la perte de communication �ventuelle avec un seul joueur
            else if (player[0].checkTime() >= TIMEOUT_VALUE) {
                cout << "No data received from player " << player[0].getName() << " - Timeout reached" << endl;
                isStarted = false;
            }
            else if (player[1].checkTime() >= TIMEOUT_VALUE) {
                cout << "No data received from player " << player[1].getName() << " - Timeout reached" << endl;
                isStarted = false;
            }
            // ici, soit le thread est bloqu� par recv, soit le mutex est accapar�

            // si le jeu continue
            if (isStarted) {
                // stockage des derni�res donn�es correctes re�ues
                player[0].pullLastReceivedData();
                player[1].pullLastReceivedData();
            }
            else {
                playerFromAddr.clear(); // suppression des joueurs
            }

            recvDataSyncMutex.unlock();
            /* --- */
            
            /* --- Gameplay --- */
            Sleep(100);
            /* --- */

            /* --- Envoi des donn�es mise � jour aux joueurs --- */
            player[0].pushCurrentPlayerData(serverSocket, player[1].getAddr());
            player[1].pushCurrentPlayerData(serverSocket, player[0].getAddr());
            /* --- */
        }
        cout << "Game stop - All players are automatically disconnected" << endl;

        /* --- fermeture des threads --- */
        // arr�t du thread de r�ception
        SOCKADDR_IN localhostAddr;
        localhostAddr.sin_family = AF_INET; localhostAddr.sin_port = htons(SERVER_PORT); localhostAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST);
        sendto(serverSocket, "0", 1, 0, (SOCKADDR*)&localhostAddr, sizeof(localhostAddr)); // envoi d'un datagramme quelconque
        // en local host pour d�bloquer la fonction recv du thread une fois 
        stop_flag_recvPlayerDataThread = true;
        recvPlayerDataThread.join();
        /* --- */
    }

    closesocket(serverSocket);
    WSACleanup();
    return 0;
}

void recvPlayerData() {
    /* --- R�cup�ration des donn�es re�ues des joueurs --- */
    ClientToServer_Position_TypeDef receiptBuffer; // buffer de r�ception de la position
    
    while (!stop_flag_recvPlayerDataThread) {
        recvDataSyncMutex.lock(); // attente de la lib�ration du mutex et lock du mutex

        int nbrBytesReceived = recvfrom(serverSocket, (char*)&receiptBuffer, sizeof(receiptBuffer),
            0, (SOCKADDR*)&clientAddr, &clientAddrSize);

        // si il s'agit d'un des deux joueurs qui a envoy� des donn�es
        if (playerFromAddr.count(clientAddr)) {
            // si il s'agit bien des donn�es de position formatt�es correctement
            if ((nbrBytesReceived == sizeof(receiptBuffer)) && (receiptBuffer.heading == POSITION_HEADING)) {
                playerFromAddr[clientAddr]->setLastReceivedData(receiptBuffer);
                playerFromAddr[clientAddr]->dataAreReceived();
            }
        }

        recvDataSyncMutex.unlock();
        this_thread::sleep_for(chrono::milliseconds(10)); // permet aux autres thread de laisser le temps d'utiliser la variable des donn�es r�cup�r�es
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
