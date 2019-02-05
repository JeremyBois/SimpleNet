#include "simpleNET.hpp"
#include <iostream>

#include <map>


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
    Net::ServerSocket servSoc(7777);
    if (servSoc.Listen())
    {
        // Blocking test
        // // Init a buffer
        // char buffer[2048] = "Bonjour Client.";
        // Net::SimpleSocket clientSoc = servSoc.Accept();

        // // Define communication protocol
        // Net::TextProtocol myProto(clientSoc);

        // // Send first message
        // printf("Send : %s  (size = %d)\n", buffer, strlen(buffer));
        // myProto.Send(buffer);

        // // Change content of buffer
        // buffer[strlen(buffer)] = '\t';
        // buffer[16] = 'n';
        // buffer[17] = 'e';
        // buffer[18] = 'w';

        // // Send another with same protocol
        // printf("Send : %s  (size = %d)\n", buffer, strlen(buffer));
        // myProto.Send(buffer);


        // SELECT
        // Init a buffer
        char buffer[2048] = {0};
        char bufferSend[2048] = "Bonjour Client.";

        // // Mark socket as non-blocking
        // servSoc.MarkAsNonBlocking();

        // Init descriptor sets
        fd_set readSet;     // temp file descriptor list for select()
        fd_set masterSet;   // master file descriptor list unchanged

        // Keep track of biggest file descriptor
        unsigned int fdmax = servSoc.GetID();

        FD_ZERO(&masterSet);
        FD_ZERO(&readSet);

        // Keep track of connection attempts
        FD_SET(servSoc.GetID(), &masterSet);

        // Keep track of sockets
        typedef std::map<int, Net::SimpleSocket> SocketMap;
        typedef std::pair<int, Net::SimpleSocket> SocketPair;
        SocketMap clientSockets;

        while(true)
        {
            // Copy master to read used by select
            // as select change it
            readSet = masterSet;

            // @TODO Change namespace for Select
            if (Net::SimpleSocket::Select(fdmax + 1, &readSet, nullptr, nullptr, nullptr) != SOCKET_ERROR)
            {
                // run through the existing connections looking for data to read
                for (unsigned int i = 0; i <= fdmax; i++)
                {
                    if (FD_ISSET(i, &readSet))
                    {
                        // Got one !
                        if (i == servSoc.GetID())
                        {
                            // A client want to connect
                            Net::SimpleSocket clientSoc = servSoc.Accept();

                            // Move do not copy it to avoid closing the socket
                            clientSockets.emplace(clientSoc.GetID(), std::move(clientSoc));

                            // Add to master set
                            FD_SET(clientSoc.GetID(), &masterSet);

                            // Update max descriptor
                            if (clientSoc.GetID() > fdmax)
                            {
                                fdmax = clientSoc.GetID();
                            }
                        }
                        // else
                        // {
                        //     // Data to send / receive
                        //     // Receive using Text protocol
                        //     Net::TextProtocol myProto(clientSockets[i]);
                        //     int result = myProto.Receive(buffer);

                        //     if (result == 0 || result == SOCKET_ERROR)
                        //     {
                        //         // Client deconnected or error occurs
                        //         clientSockets.erase(i);
                        //         FD_CLR(i, &masterSet);
                        //     }
                        //     else
                        //     {
                        //         // Client try talking !!
                        //         printf("Receive: %s \n", buffer);

                        //         // Send message to all other clients
                        //         for(unsigned int j = 0; j <= fdmax; j++)
                        //         {
                        //             if (FD_ISSET(j, &masterSet) &&
                        //                 j != servSoc.GetID() &&
                        //                 j != i)
                        //             {
                        //                 // Send using Text protocol
                        //                 Net::TextProtocol myProto(clientSockets[j]);
                        //                 myProto.Send(bufferSend);
                        //             }
                        //         }
                        //     }
                        // }
                    }
                }
            }
        }
    }
    else
    {
        printf("OUPS");
    }



    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
