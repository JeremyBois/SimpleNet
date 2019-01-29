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

    // Connect to local server (server must be start first)
    Net::ClientSocket mySocket;
    if (mySocket.Connect("127.0.0.1", 7777))
    {
        // Init a buffer
        char buffer[16];

        int length = mySocket.Receive(buffer, strlen(buffer));

        printf("Receive: %s \n", buffer);
    }

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
