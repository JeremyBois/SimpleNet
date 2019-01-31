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
        char buffer[2048] = "Bonjour Client.";
        Net::SimpleSocket clientSoc = mySocket.Accept();

        // Define communication protocol
        Net::TextProtocol myProto(clientSoc);

        // Send first message
        printf("Send : %s  (size = %d)\n", buffer, strlen(buffer));
        myProto.Send(buffer);

        // Change content of buffer
        buffer[strlen(buffer)] = '\t';
        buffer[16] = 'n';
        buffer[17] = 'e';
        buffer[18] = 'w';

        // Send another with same protocol
        printf("Send : %s  (size = %d)\n", buffer, strlen(buffer));
        myProto.Send(buffer);
    }
    else
    {
        printf("OUPS");
    }



    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
