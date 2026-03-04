//
// Created by PCS on 2/27/2026.
//

#include "../../include/network/TCPServer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

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

    char buffer[4096];

    while (true) {
        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

        if (bytesReceived <= 0)
            break;

        buffer[bytesReceived] = '\0';

        std::cout << buffer << std::endl;
    }

    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return true;
}