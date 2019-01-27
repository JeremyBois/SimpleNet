#include "Socket/ServerSocket.hpp"
#include "Socket/SimpleSocket.hpp"


#include "Tools.hpp"

// @TODO Handle error code

namespace simpleNET
{
    ServerSocket::ServerSocket(int port)
        : AbstractSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
    {
        SOCKADDR_IN sockAdress;
        memset(&sockAdress, 0, sizeof(sockAdress)); // make sure the struct is empty

        sockAdress.sin_family = AF_INET;
        sockAdress.sin_addr.s_addr = INADDR_ANY;
        // Handle difference between little-endian and big-endian
        //https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
        sockAdress.sin_port = htons(port);

        // Link socket to communication port
        if (bind(GetID(), (SOCKADDR *)&sockAdress, sizeof(sockAdress)) != 0)
        {
            fprintf(stderr, "Server binding error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
        }

        // Now ready to be called
        Listen();
    }

    void ServerSocket::Listen()
    {
        if  (listen(GetID(), SOMAXCONN))
        {
            fprintf(stderr, "Server Listen error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
        }
    }

    SimpleSocket ServerSocket::Accept()
    {
        // Check server socket is defined
        if (GetID() == INVALID_SOCKET)
        {
            fprintf(stderr, "Server socket not set error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
        }

        // Client calling informations
        struct sockaddr_storage clientAdress;
        socklen_t addr_size = sizeof(clientAdress);

        // Blocking call (Please someone can call me)
        SOCKET clientSoc = accept(GetID(), (sockaddr*)&clientAdress, &addr_size);
        if (clientSoc == INVALID_SOCKET)
        {
            closesocket(clientSoc);
            fprintf(stderr, "Server Accept error  (%d)\n",
                    Tools::GetLastErrorCodeID());

            // Bad socket returned
            return SimpleSocket(INVALID_SOCKET);
        }

        printf("New Client now connected\n");
        return SimpleSocket(clientSoc);
    }
}
