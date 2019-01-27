#include "Socket/AbstractSocket.hpp"

#include <string>
#include <utility>

namespace simpleNET
{
    AbstractSocket::AbstractSocket(SOCKET socketID)
        : _socketID(socketID)
    {
    }

    // Move semantics
    AbstractSocket::AbstractSocket(AbstractSocket &&otherSocket) noexcept
    {
        otherSocket.swap(*this);
    }

    AbstractSocket &AbstractSocket ::operator=(AbstractSocket &&otherSocket) noexcept
    {
        otherSocket.swap(*this);
        return *this;
    }

    void AbstractSocket::swap(AbstractSocket &otherSocket) noexcept
    {
        std::swap(_socketID, otherSocket._socketID);
    }

    // Methods
    void AbstractSocket::Init()
    {
    }

    void AbstractSocket::Close()
    {
        // Call close on unix systems
        closesocket(_socketID);
    }


    int AbstractSocket::GetTCPAdresses(const std::string &ipAdress, int port, addrinfo *&infosContainer)
    {
        // Find informations
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
        hints.ai_flags = AI_PASSIVE;      // fill in my IP for me
        int status = getaddrinfo(ipAdress.c_str(),
                                 std::to_string(port).c_str(),
                                 &hints,
                                 &infosContainer);
        return status;
    }

    int AbstractSocket::GetUDPAdresses(const std::string &ipAdress, int port, addrinfo *&infosContainer)
    {
        // Find informations
        struct addrinfo hints;
        memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
        hints.ai_family = AF_UNSPEC;      // don't care IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM;   // UDP stream sockets
        hints.ai_flags = AI_PASSIVE;      // fill in my IP for me
        int status = getaddrinfo(ipAdress.c_str(),
                                 std::to_string(port).c_str(),
                                 &hints,
                                 &infosContainer);
        return status;
    }

    void AbstractSocket::DisplayAdresses(const addrinfo *pinfo)
    {
        const struct addrinfo *pCurrentInfo;
        char ipstr[INET6_ADDRSTRLEN];
        char ipstr2[INET6_ADDRSTRLEN];

        for (pCurrentInfo = pinfo; pCurrentInfo != nullptr; pCurrentInfo = pCurrentInfo->ai_next)
        {
            void *addr;
            char *ipver;

            // Get the pointer to the address itself
            if (pCurrentInfo->ai_family == AF_INET)
            {
                // IPv4
                struct sockaddr_in *ipv4 = (struct sockaddr_in *)pCurrentInfo->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            }
            else
            {
                // IPv6
                struct sockaddr_in6 *ipv6 = (struct sockaddr_in6 *)pCurrentInfo->ai_addr;
                addr = &(ipv6->sin6_addr);
                ipver = "IPv6";
            }
            // InetPton(address_family, string_to_be_converted,
            //          buffer_to_store_the_converted_string);
            inet_pton(pCurrentInfo->ai_family, (PCSTR)ipstr, ipstr2);
            printf(" Value: %ul\t", (unsigned int)ipstr2);

            // InetNtop(address_family, IP_address_in_network_byte_to_convert_to_a_string,
            //          buffer_to_store_the_IP_address_string, the_IP_string_length_in_character);
            inet_ntop(pCurrentInfo->ai_family, addr, ipstr, sizeof(ipstr));
            printf("%s: %s\n", ipver, ipstr);
        }
    }

} // namespace simpleNET
