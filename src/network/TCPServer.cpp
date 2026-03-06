//
// Created by PCS on 2/27/2026.
//

#include "../../include/network/TCPServer.h"
#include "../../include/processing/Point.h"
#include "../../include/processing/LandmarkNormalizer.h"

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#include "features/FeatureExtractor.h"
#include "json/json.hpp"

using json = nlohmann::json;

#pragma comment(lib, "Ws2_32.lib")

TCPServer::TCPServer(int port) : port(port) {}

bool TCPServer::start()
{
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

    clientSocket = accept(serverSocket, NULL, NULL);

    closesocket(serverSocket);

    if(clientSocket == INVALID_SOCKET)
    {
        std::cout << "Accept failed\n";
        return false;
    }

    std::cout << "Python connected.\n";

    return true;
}

std::vector<double> TCPServer::receiveVector()
{
    static std::string accumulator;
    char buffer[4096];

    while(true)
    {
        size_t pos = accumulator.find('\n');

        if(pos != std::string::npos)
        {
            std::string line = accumulator.substr(0,pos);
            accumulator.erase(0,pos+1);

            try
            {
                json j = json::parse(line);

                if(j.size() != 63)
                    continue;

                std::vector<Point> landmarks(21);

                for(int i = 0; i < 21; i++)
                {
                    landmarks[i].x = j[i*3+0];
                    landmarks[i].y = j[i*3+1];
                    landmarks[i].z = j[i*3+2];
                }

                LandmarkNormalizer::normalize(landmarks);

                std::vector<float> features =
                        FeatureExtractor::extract(landmarks);

                return std::vector<double>(features.begin(), features.end());
            }
            catch(...)
            {
                continue;
            }
        }

        int bytesReceived = recv(clientSocket, buffer, sizeof(buffer)-1, 0);

        if(bytesReceived == 0)
        {
            std::cout << "Python disconnected\n";
            return {};
        }

        if(bytesReceived < 0)
        {
            continue;
        }

        buffer[bytesReceived] = '\0';
        accumulator += buffer;
    }
}

void TCPServer::stop()
{
    closesocket(clientSocket);
    WSACleanup();
}