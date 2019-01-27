#include "simpleNET.hpp"
#include <iostream>

namespace Net = simpleNET;


int main(int argc, char **argv)
{
    // Init
    Net::SimpleNetInit();

    // Test purpose
    std::cout << "I'm the SERVER" << std::endl;

    // Test lib import
    Net::Client aClient;
    Net::Server aServer;

    Net::ServerSocket mySocket(7777);
    mySocket.Accept();

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
