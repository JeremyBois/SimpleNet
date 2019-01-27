#include "Socket/ClientSocket.hpp"

#include "Tools.hpp"

// @TODO Handle error code

namespace simpleNET
{
    ClientSocket::ClientSocket(std::string const &ipAdress, int port)
        : SimpleSocket(INVALID_SOCKET)
    {
        // Find informations
        struct addrinfo *servinfo;
        int status = GetTCPAdresses(ipAdress, port, servinfo);
        if  (status != 0)
        {
            fprintf(stderr, "GetTCPAdresses error  (%d)\n",
                    Tools::GetLastErrorCodeID());
        }
        else
        {
            // Debug
            DisplayAdresses(servinfo);

            // Try to connect
            if (this->Connect(servinfo))
            {
                fprintf(stdout, "Connection establised\n");
            }
            else
            {
              fprintf(stderr, "Not able to create a connection (%d)\n",
                      Tools::GetLastErrorCodeID());
              Close();
            }
        }

        // Free memory of linked list
        if (servinfo != nullptr)
        {
            freeaddrinfo(servinfo);
        }
    }

    bool ClientSocket::Connect(const addrinfo* adressInfos)
    {
        const addrinfo *addr;
        SOCKET soc;

        for (addr = adressInfos; addr != nullptr; addr = addr->ai_next)
        {
            soc = socket(addr->ai_family,
                         addr->ai_socktype,
                         addr->ai_protocol);

            if (soc == INVALID_SOCKET)
            {
                fprintf(stderr, "Socket construction error (%d)\n",
                        Tools::GetLastErrorCodeID());
                closesocket(soc);
            }
            else
            {
                // Connect
                if (connect(soc, addr->ai_addr, addr->ai_addrlen) != SOCKET_ERROR)
                {
                    this->SetID(soc);
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

