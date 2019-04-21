#include "Tools.hpp"

#include <string>


namespace simpleNET
{

    int Tools::GetLastErrorCodeID()
    {
        #if defined _WIN32
            // Assume Windows system
            return WSAGetLastError();
        #else
            // Assume Unix system
            return errno;
        #endif
    }

    // std::string Tools::GetLastErrorMsg(int msgID)
    // {
    //     return "Error";
    // }

    // void Tools::PrintLastErrorMsg()
    // {

    // }

    int Tools::GetAdressAndService(const sockaddr *psockAdress, size_t sizeStruct,
                                   std::string &adress, std::string &service)
    {
        // @TODO check if also in linux (define in Ws2tcpip.h)

        char bufAdress[INET6_ADDRSTRLEN];
        char bufService[NI_MAXSERV];

        size_t sizeAdress = sizeof(bufAdress);
        size_t sizeService = sizeof(bufService);

        int dwRetval = getnameinfo(psockAdress, sizeStruct,
                                   bufAdress, sizeAdress,
                                   bufService, sizeService,
                                   NI_NUMERICHOST | NI_NUMERICSERV);

        adress.assign(bufAdress, sizeAdress);
        service.assign(bufService, sizeService);

        return dwRetval;
    }

    int Tools::GetTCPAdresses(const std::string &ipAdress, int port, ADDRESS_FAMILY family,
                              addrinfo *&infosContainer)
    {
        // Find informations
        addrinfo hints;
        memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
        hints.ai_family = family;         // IPv4 or IPv6
        hints.ai_socktype = SOCK_STREAM;  // TCP stream sockets
        hints.ai_protocol = IPPROTO_TCP;  // Force protocol ??
        hints.ai_flags = AI_PASSIVE;      // fill in my IP for me
        int status = getaddrinfo(ipAdress.c_str(),
                                 std::to_string(port).c_str(),
                                 &hints,
                                 &infosContainer);
        return status;
    }

    int Tools::GetUDPAdresses(const std::string &ipAdress, int port, ADDRESS_FAMILY family,
                              addrinfo *&infosContainer)
    {
        // Find informations
        addrinfo hints;
        memset(&hints, 0, sizeof(hints)); // make sure the struct is empty
        hints.ai_family = family;         // IPv4 or IPv6
        hints.ai_socktype = SOCK_DGRAM;   // UDP stream sockets
        hints.ai_protocol = IPPROTO_UDP;  // Force protocol ??
        hints.ai_flags = AI_PASSIVE;      // fill in my IP for me
        int status = getaddrinfo(ipAdress.c_str(),
                                 std::to_string(port).c_str(),
                                 &hints,
                                 &infosContainer);
        return status;
    }

    void Tools::DisplayAdresses(const addrinfo *pinfo)
    {
        const addrinfo *pCurrentInfo;
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
                sockaddr_in *ipv4 = (sockaddr_in *)pCurrentInfo->ai_addr;
                addr = &(ipv4->sin_addr);
                ipver = "IPv4";
            }
            else
            {
                // IPv6
                sockaddr_in6 *ipv6 = (sockaddr_in6 *)pCurrentInfo->ai_addr;
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

    void *Tools::GetSocketAdress(const sockaddr *psockAdress)
    {
        if (psockAdress->sa_family == AF_INET)
        {
            // IPv4
            return &(((sockaddr_in *)psockAdress)->sin_addr);
        }
        else if (psockAdress->sa_family == AF_INET6)
        {
            // IPv6
            return &(((sockaddr_in6 *)psockAdress)->sin6_addr);
        }

        return nullptr;
    }

    unsigned short Tools::GetSocketPort(const sockaddr *psockAdress)
    {
        if (psockAdress->sa_family == AF_INET)
        {
            // IPv4
            return ((sockaddr_in *)psockAdress)->sin_port;
        }
        else if (psockAdress->sa_family == AF_INET6)
        {
            // IPv6
            return ((sockaddr_in6 *)psockAdress)->sin6_port;
        }

        return 0u;
    }


} // namespace simpleNET
