//
// Created by PCS on 2/27/2026.
//

#ifndef HAND_SIGN_TCPSERVER_H
#define HAND_SIGN_TCPSERVER_H


#pragma once
#include <vector>

class TCPServer {
public:
    TCPServer(int port);
    bool start();
    std::vector<double> receiveVector();
private:
    int port;
};

#endif //HAND_SIGN_TCPSERVER_H