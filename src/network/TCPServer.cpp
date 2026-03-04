//
// Created by PCS on 2/27/2026.
//

#include "../../include/network/TCPServer.h"
#include "../../include/processing/Point.h"
#include "../../include/processing/LandmarkNormalizer.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "json/json.hpp"
using json = nlohmann::json;

#pragma comment(lib, "Ws2_32.lib")

TCPServer::TCPServer(int port) : port(port) {}

bool TCPServer::start() {
    WSADATA wsaData;
    WSAStartup(MAKEWORD(2,2), &wsaData);

    SOCKET serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddr{};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(port);
    serverAddr.sin_addr.s_addr = INADDR_ANY;

    bind(serverSocket, (SOCKADDR*)&serverAddr, sizeof(serverAddr));
    listen(serverSocket, 1);

    std::cout << "Waiting for Python connection...\n";

    SOCKET clientSocket = accept(serverSocket, NULL, NULL);

    std::cout << "Python connected.\n";

    std::string accumulator;
    char buffer[4096];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);

        if (bytesReceived <= 0)
            break;

        buffer[bytesReceived] = '\0';
        accumulator += buffer;

        size_t pos;
        while ((pos = accumulator.find('\n')) != std::string::npos) {

            std::string line = accumulator.substr(0, pos);
            accumulator.erase(0, pos + 1);

            try {
                json j = json::parse(line);

                if (j.size() == 63) {

                    std::vector<Point> landmarks(21);

                    for (int i = 0; i < 21; i++) {
                        landmarks[i].x = j[i * 3 + 0];
                        landmarks[i].y = j[i * 3 + 1];
                        landmarks[i].z = j[i * 3 + 2];
                    }

                    LandmarkNormalizer::normalize(landmarks);

                    // DEBUG PRINT AFTER NORMALIZATION
                    std::cout << "Index tip after normalize: "
                              << landmarks[8].x << ", "
                              << landmarks[8].y << ", "
                              << landmarks[8].z << "\n";
                }

            } catch (const std::exception& e) {
                std::cout << "JSON parse error: " << e.what() << "\n";
            }
        }
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return true;
}