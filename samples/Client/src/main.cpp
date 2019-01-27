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
    Net::Client aClient;
    Net::Server aServer;

    // Try to connect to a website
    // Net::ClientSocket mySocket("www.example.com", 7777);
    Net::ClientSocket mySocket("127.0.0.1", 7777);

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
