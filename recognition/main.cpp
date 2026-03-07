#include "network/TCPServer.h"
#include <iostream>

int main()
{
    TCPServer server(3000);

    if(!server.start())
    {
        std::cout << "Server failed to start\n";
        return 1;
    }

    while(true)
    {
        std::string sign = server.receiveSign();

        if(sign.empty())
            break;

        std::cout << "Received sign: " << sign << std::endl;
    }

    server.stop();

    return 0;
}