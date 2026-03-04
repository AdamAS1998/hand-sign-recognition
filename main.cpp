
#include "network/TCPServer.h"


int main() {
    TCPServer server(3000);
    server.start();
    return 0;
}