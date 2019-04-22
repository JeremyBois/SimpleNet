#include "Socket/TCPsocket.hpp"

#include "Tools.hpp"


namespace simpleNET
{
    TCPsocket::TCPsocket(bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_STREAM, IPPROTO_TCP))
    {

    }

    TCPsocket::TCPsocket(SOCKET socketID)
        : SimpleSocket(socketID)
    {

    }

    TCPsocket::TCPsocket(std::string const &ipAddress, int port, bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_STREAM, IPPROTO_TCP))
    {
        Bind(ipAddress, port, ipV6 ? AF_INET6 : AF_INET);
    }

    TCPsocket::TCPsocket(int port, bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_STREAM, IPPROTO_TCP))
    {
        BindAll(port, ipV6 ? AF_INET6 : AF_INET);
    }

    int TCPsocket::SendImplementation(char* buffer, int bufferSize,
                                      sockaddr* socAdress, socklen_t socAdressLength,
                                      int flags)
    {
        // Message not always send in one block
        int dataSent = 0;
        int remainingCount = bufferSize;

        while(dataSent < bufferSize)
        {
            // Send in TCP == SendTo without socAdress
            int nbCharSent = sendto(GetID(), buffer + dataSent, remainingCount,
                                    flags, socAdress, socAdressLength);
            // int nbCharSent = send(GetID(), buffer + dataSent, remainingCount, flags);
            // printf("-- %d -- \n", nbCharSent);

            if (nbCharSent == SOCKET_ERROR)
            {
                // @TODO Handle error code
                fprintf(stderr, "Send operation failed (%d / %d sent) with error # %ld\n",
                        dataSent, bufferSize, Tools::GetLastErrorCodeID());
                break;
            }
            else
            {
                dataSent += nbCharSent;
                remainingCount -= nbCharSent;
            }
        }

        return dataSent;

    }

    int TCPsocket::ReceiveImplementation(char* buffer, int bufferSize,
                                            sockaddr* socAdress, socklen_t* socAdressLength,
                                            int flags)
    {
        int dataRead = 0;
        int dataMax = bufferSize - 1;
        int remainingCount = bufferSize;

        while(dataRead < dataMax)
        {
            // Recv in TCP == RecvFrom without socAdress
            int nbCharRecv = recvfrom(GetID(), buffer + dataRead, remainingCount,
                                      flags, socAdress, socAdressLength);
            // int nbCharRecv = recv(GetID(), buffer + dataRead, remainingCount, flags);
            // printf("-- %d -- \n", nbCharRecv);

            if (nbCharRecv == SOCKET_ERROR)
            {
                int resultCode = Tools::GetLastErrorCodeID();
                if (resultCode != 10035)
                {
                    // @TODO Handle error code
                    fprintf(stderr, "Receive operation failed (%d / %d received) with error # %ld\n",
                            dataRead, bufferSize, Tools::GetLastErrorCodeID());
                }

                break;
            }
            else if (nbCharRecv == 0)
            {
                // Message over
                break;
            }
            else
            {
                dataRead += nbCharRecv;
                remainingCount -= nbCharRecv;
            }
        }

        return dataRead;
    }

    bool TCPsocket::Connect(std::string const &ipAddress, int port, bool ipV6)
    {
        // Check is server socket is defined
        if (GetID() == INVALID_SOCKET)
        {
            fprintf(stderr, "Socket not set\n");
            Close();
        }

        // Flag controlling state
        bool connected = false;

        // Find informations
        addrinfo* servInfos;
        int status = Tools::GetTCPAdresses(ipAddress, port, ipV6 ? AF_INET6 : AF_INET,
                                           servInfos);
        if (status != 0)
        {
            fprintf(stderr, "GetTCPAdresses error (%d)\n",
                    Tools::GetLastErrorCodeID());
        }
        else
        {
            // Debug
            Tools::DisplayAdresses(servInfos);

            // Try to connect
            addrinfo servInfoUsed = *servInfos;
            if (this->_Connect(servInfos, servInfoUsed))
            {
                // Extract data from server
                std::string serverName;
                std::string servPort;
                int dwRetval = Tools::GetAdressAndService((sockaddr *)servInfoUsed.ai_addr,
                                                          sizeof(servInfoUsed),
                                                          serverName, servPort);
                if (dwRetval != 0)
                {
                    fprintf(stderr, "getnameinfo failed with error # %ld\n",
                            Tools::GetLastErrorCodeID());
                }
                else
                {
                    fprintf(stdout,
                            "Connection establised with %s (using port %s)\n",
                            serverName.c_str(), servPort.c_str());
                }

                connected = true;
            }
            else
            {
                fprintf(stderr, "Not able to create a connection (%d)\n",
                        Tools::GetLastErrorCodeID());
                Close();
            }
        }

        // Free memory of linked list
        if (servInfos != nullptr)
        {
            freeaddrinfo(servInfos);
        }

        return connected;
    }

    bool TCPsocket::_Connect(const addrinfo *paddrInfos, addrinfo &pusedInfo)
    {
        const addrinfo *paddr;

        for (paddr = paddrInfos; paddr != nullptr; paddr = paddr->ai_next)
        {
            // Connect
            if (connect(GetID(), paddr->ai_addr, paddr->ai_addrlen) != SOCKET_ERROR)
            {
                pusedInfo = *paddr;
                return true;
            }

            printf("Connection error: %d\n", Tools::GetLastErrorCodeID());
        }

        return false;
    }

    bool TCPsocket::Listen()
    {
        if (listen(GetID(), SOMAXCONN))
        {
            fprintf(stderr, "Socket Listen error (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
            return false;
        }

        return true;
    }

    TCPsocket TCPsocket::Accept()
    {
        // Check is server socket is defined
        if (GetID() == INVALID_SOCKET)
        {
            fprintf(stderr, "Socket not set\n");
            Close();
        }

        // Used to get client informations
        sockaddr_storage clientStorage;
        socklen_t addr_size = sizeof(clientStorage);

        // Blocking call
        SOCKET clientSoc = accept(GetID(), (sockaddr *)&clientStorage, &addr_size);
        if (clientSoc == INVALID_SOCKET)
        {
            closesocket(clientSoc);
            fprintf(stderr, "Accept client failed with error (%d)\n",
                    Tools::GetLastErrorCodeID());

            // Bad socket returned
            return TCPsocket(INVALID_SOCKET);
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

        return TCPsocket(clientSoc);
    }

}// namespace simpleNET
