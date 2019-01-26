#include "simpleNET.hpp"
#include <iostream>


namespace Net = simpleNET;


int main(int argc, char **argv)
{
    // Init
    Net::SimpleNetInit();

    // Test purpose
    std::cout << "I'm the CLIENT" << std::endl;


    // Test lib import
    Net::Socket aSocket;
    Net::Client aClient;
    Net::Server aServer;

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
