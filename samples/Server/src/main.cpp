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

    // Start server and wait for client
    Net::ServerSocket mySocket(7777);
    if (mySocket.Listen())
    {
        // Init a buffer
        char buffer[16] = "Bonjour Client.";
        printf("%s -- sent (size = %d)\n", buffer, strlen(buffer));

        Net::SimpleSocket clientSoc = mySocket.Accept();
        clientSoc.Send(buffer, strlen(buffer));
    }
    else
    {
        printf("OUPS");
    }



    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
