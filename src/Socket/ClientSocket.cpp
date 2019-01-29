#include "Socket/ClientSocket.hpp"

#include "Tools.hpp"

// @TODO Handle error code

namespace simpleNET
{
    ClientSocket::ClientSocket()
        : SimpleSocket(INVALID_SOCKET)
    {
    }

    bool ClientSocket::Connect(std::string const &ipAdress, int port)
    {
        // Flag controlling state
        bool connected = false;


        // Find informations
        struct addrinfo *servInfos;
        int status = Tools::GetTCPAdresses(ipAdress, port, servInfos);
        if (status != 0)
        {
            fprintf(stderr, "GetTCPAdresses error  (%d)\n",
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

    bool ClientSocket::_Connect(const addrinfo *paddrInfos, addrinfo& pusedInfo)
    {
        const addrinfo* paddr;
        SOCKET soc;

        for (paddr = paddrInfos; paddr != nullptr; paddr = paddr->ai_next)
        {
            soc = socket(paddr->ai_family,
                         paddr->ai_socktype,
                         paddr->ai_protocol);

            if (soc == INVALID_SOCKET)
            {
                fprintf(stderr, "Socket construction error (%d)\n",
                        Tools::GetLastErrorCodeID());
                closesocket(soc);
            }
            else
            {
                // Connect
                if (connect(soc, paddr->ai_addr, paddr->ai_addrlen) != SOCKET_ERROR)
                {
                    this->SetID(soc);
                    pusedInfo = *paddr;
                    return true;
                }

                printf("Connection error: %d\n", Tools::GetLastErrorCodeID());
                closesocket(soc);
                soc = INVALID_SOCKET;
            }
        }

        return false;
    }
}

