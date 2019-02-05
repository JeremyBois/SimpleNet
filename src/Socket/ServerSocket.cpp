#include "Socket/ServerSocket.hpp"
#include "Socket/SimpleSocket.hpp"

#include <string>
#include "Tools.hpp"

// @TODO Handle error code
// @TODO IPV4 only for now


namespace simpleNET
{
    ServerSocket::ServerSocket(const std::string& ipAdress, int port)
        : AbstractSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
    {
        sockaddr_in sockAdress;
        memset(&sockAdress, 0,
               sizeof(sockAdress)); // make sure the struct is empty

        sockAdress.sin_family = AF_INET;
        // Specific adress
        inet_pton(PF_INET, ipAdress.c_str(), &sockAdress.sin_addr.s_addr);
        // Handle difference between little-endian and big-endian
        // https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
        sockAdress.sin_port = htons(port);

        // Bind it to this adress
        Bind(sockAdress);

        // Now ready to be called
    }


    ServerSocket::ServerSocket(int port)
        : AbstractSocket(socket(AF_INET, SOCK_STREAM, IPPROTO_TCP))
    {
        sockaddr_in sockAdress;
        memset(&sockAdress, 0, sizeof(sockAdress)); // make sure the struct is empty
        sockAdress.sin_family = AF_INET;
        // Bound to all interfaces (0.0.0.0)
        // https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
        sockAdress.sin_addr.s_addr = htonl(INADDR_ANY);
        // Handle difference between little-endian and big-endian
        //https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
        sockAdress.sin_port = htons(port);

        // Bind it to this adress
        Bind(sockAdress);

        // Now ready to be called
    }

    void ServerSocket::Bind(sockaddr_in& sockAdress)
    {
        // Link socket to communication port
        if (bind(GetID(), (SOCKADDR *)&sockAdress, sizeof(sockAdress)) != 0)
        {
            fprintf(stderr, "Server binding error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
        }
        else
        {
            // Find informations
            std::string name;
            std::string portUsed;
            int dwRetval = Tools::GetAdressAndService((sockaddr *)&sockAdress,
                                                      sizeof(sockAdress), name, portUsed);

            fprintf(stdout, "Server bind to port %s (%s)\n",
                    portUsed.c_str(), name.c_str());
        }
    }

    bool ServerSocket::Listen()
    {
        if  (listen(GetID(), SOMAXCONN))
        {
            fprintf(stderr, "Server Listen error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
            return false;
        }

        return true;
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
        sockaddr_storage clientStorage;
        socklen_t addr_size = sizeof(clientStorage);

        // Blocking call (Please someone can call me)
        SOCKET clientSoc = accept(GetID(), (sockaddr*)&clientStorage, &addr_size);
        if (clientSoc == INVALID_SOCKET)
        {
            closesocket(clientSoc);
            fprintf(stderr, "Server Accept error  (%d)\n",
                    Tools::GetLastErrorCodeID());

            // Bad socket returned
            return SimpleSocket(INVALID_SOCKET);
        }

        std::string clientName;
        std::string clientPort;
        int dwRetval = Tools::GetAdressAndService((sockaddr *)&clientStorage,
                                                  addr_size, clientName, clientPort);

        if (dwRetval != 0)
        {
            fprintf(stderr, "getnameinfo failed with error # %ld\n",
                    Tools::GetLastErrorCodeID());
        }
        else
        {
            fprintf(stdout, "Connection establised with %s\n",
                    clientName.c_str());
        }

        return SimpleSocket(clientSoc);
    }
}
