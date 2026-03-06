//
// Created by PCS on 2/27/2026.
//

#ifndef HAND_SIGN_TCPSERVER_H
#define HAND_SIGN_TCPSERVER_H


#pragma once
#include <vector>
#include <winsock2.h>

class TCPServer {
public:
    TCPServer(int port);
    bool start();
    std::vector<double> receiveVector();
    void stop();
private:
    int port;
    SOCKET clientSocket;
};

#endif //HAND_SIGN_TCPSERVER_H