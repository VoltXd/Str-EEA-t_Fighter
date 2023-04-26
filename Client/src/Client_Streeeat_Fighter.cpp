// Client_Street_Fighter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#define _WINSOCK_DEPRECATED_NO_WARNINGS

#include "Client_Streeeat_Fighter.hpp"

int main(int argc, char** argv) {
    WSADATA wsaData;

    bool isStarted = false; // vrai lorsque le jeu est lancé

    // Initialisation Winsock version 2.2
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0) {
        std::cout << "Client: WSAStartup failed with error" << WSAGetLastError() << std::endl;
        return EXIT_FAILURE;
    }
    else {
        std::cout << "Client: The Winsock DLL status is" << wsaData.szSystemStatus << std::endl;
    }

    // Création du socket pour envoyer des données au serveur (port non occupé choisi automatiquement)
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
        ServerToClient_Start_TypeDef startData = { 0, {0} };

        /* --- Demande du nom du joueur --- */
        std::cout << "Player Name (max char. = " << MAX_NBR_LETTERS_IN_PLAYERNAME << ") : ";
        std::cin >> connectionData.playerName;
        player = new Player(connectionData.playerName); // création du joueur 
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
        opponent = new Player(startData.opponentName);
        std::cout << "Opponent found : " << startData.opponentName << std::endl;
        /* --- */

        std::cout << "Game start" << std::endl;
        isStarted = true;
        app = new App(SCREEN_WIDTH, SCREEN_HEIGHT);

        stop_flag_getAndSendPosThread = false;
        stop_flag_recvPlayerDataThread = false;
        std::thread sendPosThread(getAndSendPos); // lancement du thread qui récupère la position et l'envoit régulièrement au serveur
        std::thread recvPlayerDataThread(recvPlayerData); // lancement du thread qui récupère régulièrement les données reçues du serveur

        player->dataAreReceived();
        opponent->dataAreReceived();
        while (isStarted) {
            /* --- stockage des données + gestion pertes de communication --- */
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
            if (app->exit()) isStarted = false;
            app->renderClear();
            // joueur local
            app->drawRect(player->getLeftHandPos().x, player->getLeftHandPos().y, HAND_WIDTH, HAND_HEIGHT, {255,255,255,255});
            app->drawRect(player->getRightHandPos().x, player->getRightHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,255,255,255 });
            app->drawRect(player->getHeadPos().x, player->getHeadPos().y, HEAD_WIDTH, HEAD_HEIGHT, { 255,255,255,255 });
            // adversaire
            app->drawRect(opponent->getLeftHandPos().x, opponent->getLeftHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,0,0,255 });
            app->drawRect(opponent->getRightHandPos().x, opponent->getRightHandPos().y, HAND_WIDTH, HAND_HEIGHT, { 255,0,0,255 });
            app->drawRect(opponent->getHeadPos().x, opponent->getHeadPos().y, HEAD_WIDTH, HEAD_HEIGHT, { 255,0,0,255 });
            app->renderPresent();
            /* --- */
        }
        delete app;
        std::cout << "Game stop - All players are automatically disconnected" << std::endl;

        /* --- fermeture des threads --- */
        // arrêt du thread de l'envoi des positions (fin de la boucle en cours)
        stop_flag_getAndSendPosThread = true;
        sendPosThread.join();

        // arrêt du thread de réception
        stop_flag_recvPlayerDataThread = true;
        SOCKADDR_IN localhostAddr;
        int localhostAddrLength = sizeof(localhostAddr);
        getsockname(clientSocket, (SOCKADDR*)&localhostAddr, &localhostAddrLength); // récupération du port associé au socket
        localhostAddr.sin_addr.s_addr = inet_addr(LOCAL_HOST); // adresse local
        sendto(clientSocket, "0", 1, 0, (SOCKADDR*)&localhostAddr, localhostAddrLength); // envoi d'un datagramme quelconque
        // en local host pour débloquer la fonction recv du thread une fois
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
    ClientToServer_Position_TypeDef posDataToSend;
    posDataToSend.handPos[0] = Vec2D(45, 50), posDataToSend.handPos[1] = Vec2D(55, 50), posDataToSend.headPos = Vec2D(50, 40);
    posDataToSend.paused = INITIAL_GAMESTATE;

    /* --- Video processing init --- */
    cv::Mat frame, croppedHead, screenshotCalibration;
    cv::Mat gCroppedHead, gScreenshot, gFrame;
    cv::Mat rgbFrame, rgbScreenshotCalibration;
    cv::Vec3f handRgbCalibration;
    std::chrono::time_point<std::chrono::high_resolution_clock> start, end;
    bool duringCalibration = false;
    bool endCalibrationTimer = false;
    float rThreshold = 0., gThreshold = 0., bThreshold = 0.;
    // Open the defautl camera
    cv::VideoCapture cap(0);
    // Calibration ellipse's variables
    cv::Point imageCenter = cv::Point(cap.get(cv::CAP_PROP_FRAME_WIDTH) / 2, cap.get(cv::CAP_PROP_FRAME_HEIGHT) / 3);
    cv::Size ellipseSize = cv::Size(37, 50);
    cv::Scalar ellipseColor = cv::Scalar(0, 255, 0);
    cv::Point headCenter = imageCenter;
    int handSquareSize = ellipseSize.width * 3 / 2;

    // Hand calibration data
    cv::Point topLeftLhandCorner(imageCenter.x - 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2);
    cv::Point bottomRightLhandCorner(imageCenter.x - 2 * ellipseSize.width + handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize);
    cv::Point leftHandCenter((topLeftLhandCorner.x + handSquareSize / 2), (topLeftLhandCorner.y + handSquareSize / 2));
    cv::Point topLeftRhandCorner(imageCenter.x + 2 * ellipseSize.width - handSquareSize, imageCenter.y + ellipseSize.height * 3 / 2);
    cv::Point bottomRightRhandCorner(imageCenter.x + 2 * ellipseSize.width, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize);
    cv::Point rightHandCenter((topLeftRhandCorner.x + handSquareSize / 2), (topLeftRhandCorner.y + handSquareSize / 2));

    cv::Point oldLHCenter, oldRHCenter;
    if (!cap.isOpened())    {
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
        // Get the next camera video frame and pre-processes it
        cap >> frame;
        cv::flip(frame, frame, 1);

        // Color space to detect the gloves
        rgbFrame = RGBtorgb(frame);
        rgbThreshold(rgbFrame, rThreshold, gThreshold, bThreshold);
        cv::erode(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));
        cv::dilate(rgbFrame, rgbFrame, cv::getStructuringElement(cv::MORPH_RECT, cv::Size(3, 3)));

        cv::cvtColor(frame, gFrame, cv::COLOR_BGR2GRAY);
        cv::equalizeHist(gFrame, gFrame);

        // Check if the frame has been correctly received
        if (frame.empty())
            break;

        // Calibration starts when pressing the ENTER key
        if (cv::waitKey(10) == 13) {
            start = std::chrono::high_resolution_clock::now();
            end = start;
            duringCalibration = true;
            endCalibrationTimer = false;
        }

        // 5 seconds are given to strike a pose
        if (duringCalibration && std::chrono::duration_cast<std::chrono::seconds>(end - start).count() < 5) {
            end = std::chrono::high_resolution_clock::now();
            cap >> frame;
            cv::flip(frame, frame, 1);
            cv::ellipse(frame, imageCenter, ellipseSize, 0, 0, 360, ellipseColor, 5);
            cv::rectangle(frame, topLeftLhandCorner, bottomRightLhandCorner, cv::Scalar(0, 255, 0), 5);
            cv::rectangle(frame, topLeftRhandCorner, bottomRightRhandCorner, cv::Scalar(0, 255, 0), 5);
            cv::circle(frame, headCenter, 5, cv::Scalar(0, 0, 255), -1);
            cv::circle(frame, leftHandCenter, 5, cv::Scalar(0, 255, 0), -1);
            cv::circle(frame, rightHandCenter, 5, cv::Scalar(0, 255, 0), -1);
            cv::imshow("Flux vidéo de la caméra", frame);
            cv::waitKey(1);
        }

        if (std::chrono::duration_cast<std::chrono::seconds>(end - start).count() >= 5) {
            endCalibrationTimer = true;
        }

        if (duringCalibration && endCalibrationTimer) {
            duringCalibration = false;
            // Saving the current frame for processing purpose
            screenshotCalibration = frame.clone();

            cvtColor(screenshotCalibration, gScreenshot, cv::COLOR_BGR2GRAY);
            rgbScreenshotCalibration = RGBtorgb(screenshotCalibration);
            handRgbCalibration = rgbScreenshotCalibration.at<cv::Vec3f>(cv::Point(imageCenter.x - 2 * ellipseSize.width + handSquareSize / 2, imageCenter.y + ellipseSize.height * 3 / 2 + handSquareSize / 2) - cv::Point(6, 0));          
            rThreshold = handRgbCalibration[2] * (1.0 - THRESHOLD_RATIO);
            gThreshold = handRgbCalibration[1] * (1.0 + THRESHOLD_RATIO);
            bThreshold = handRgbCalibration[0] * (1.0 + THRESHOLD_RATIO);
            leftHandCenter = cv::Point((topLeftLhandCorner.x + handSquareSize / 2), (topLeftLhandCorner.y + handSquareSize / 2));
            rightHandCenter = cv::Point((topLeftRhandCorner.x + handSquareSize / 2), (topLeftRhandCorner.y + handSquareSize / 2));
            cv::equalizeHist(gScreenshot, gScreenshot);
            // Saving the cropped head
            croppedHead = headCalibration(imageCenter, ellipseSize, frame);
            cv::Rect croppedRegion(imageCenter.x - ellipseSize.width, imageCenter.y - ellipseSize.height, 2 * ellipseSize.width, 2 * ellipseSize.height);
            // Re-sizing the cropped head as small as possible to make it a convolution kernel
            croppedHead = croppedHead(croppedRegion);
            cv::cvtColor(croppedHead, gCroppedHead, cv::COLOR_BGR2GRAY);
            cv::equalizeHist(gCroppedHead, gCroppedHead);
        }

        // Tracking the head if the calibration has already been done
        if (!croppedHead.empty()) {
            headCenter = headTracking(gCroppedHead, gFrame, gScreenshot, headCenter, ellipseSize);
            oldLHCenter = leftHandCenter;
            oldRHCenter = rightHandCenter;

            leftHandCenter = handTracking(leftHandCenter, handSquareSize, rgbFrame);
            rightHandCenter = handTracking(rightHandCenter, handSquareSize, rgbFrame);
        }

        // Display the video frame with the callibration ellipse and the head center
        if (croppedHead.empty()) {
            cv::ellipse(frame, imageCenter, ellipseSize, 0, 0, 360, ellipseColor, 5);
            cv::rectangle(frame, topLeftLhandCorner, bottomRightLhandCorner, cv::Scalar(0, 255, 0), 5);
            cv::rectangle(frame, topLeftRhandCorner, bottomRightRhandCorner, cv::Scalar(0, 255, 0), 5);
        }
        cv::circle(frame, headCenter, 5, cv::Scalar(0, 0, 255), -1);
        cv::circle(frame, leftHandCenter, 5, cv::Scalar(0, 255, 0), -1);
        cv::circle(frame, rightHandCenter, 5, cv::Scalar(0, 255, 0), -1);
        cv::imshow("Flux vidéo de la caméra", frame);
        /* --- */

        /* --- mise en forme des données dans la trame d'envoi --- */
        posDataToSend.handPos[0].x = (float)leftHandCenter.x / frame.cols * 100;
        posDataToSend.handPos[0].y = (float)leftHandCenter.y / frame.rows * 100;
        posDataToSend.handPos[1].x = (float)rightHandCenter.x / frame.cols * 100;
        posDataToSend.handPos[1].y = (float)rightHandCenter.y / frame.rows * 100;

        posDataToSend.headPos.x = (float)headCenter.x / frame.cols * 100;
        posDataToSend.headPos.y = (float)headCenter.y / frame.cols * 100;
        posDataToSend.paused = IS_STARTED;
        /* --- */

        posDataToSend.date = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();

        sendto(clientSocket, (const char*)&posDataToSend, sizeof(posDataToSend), 0, (const sockaddr*)&serverAddr, serverAddrSize);
    }

    cv::destroyAllWindows();
}

void recvPlayerData() {
    unsigned long long latency = 0;

    while (!stop_flag_recvPlayerDataThread.load()) {
        ServerToClient_Data_TypeDef receiptBuffer; // buffer de réception des données d'un joueur

        int nbrBytesReceived = recvfrom(clientSocket, (char*)&receiptBuffer, sizeof(receiptBuffer),
            0, (SOCKADDR*)&serverAddr, &serverAddrSize);

        // si les données sont formattées correctement
        if (nbrBytesReceived == sizeof(receiptBuffer)) {
            switch (receiptBuffer.heading) {
            case LOCAL_PLAYER_HEADING:
                recvDataSyncMutex.lock(); // attente de la libération du mutex et lock du mutex
                player->setPrevReceivedData(player->getLastReceivedData());
                player->setLastReceivedData(receiptBuffer);
                
                // si les dernières données reçues sont des nouvelles données et non pas les mêmes (données de position uniquement)
                if (player->getLastReceivedData().date != player->getPrevReceivedData().date) {
                    // calcul de la latence
                    std::chrono::microseconds date(player->getLastReceivedData().date);
                    latency = (std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()) - date).count();
                    std::cout << "Latency : " << latency << std::endl;

                    // mise à jour des paramètres pour le déplacement
                    player->setAutoShiftingParameters();
                }

                player->dataAreReceived();
                recvDataSyncMutex.unlock();
                break;
            case OPPOSING_PLAYER_HEADING:
                recvDataSyncMutex.lock(); // attente de la libération du mutex et lock du mutex
                opponent->setPrevReceivedData(opponent->getLastReceivedData());
                opponent->setLastReceivedData(receiptBuffer);

                if (opponent->getLastReceivedData().date != opponent->getPrevReceivedData().date) {
                    // mise à jour des paramètres pour le déplacement
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
