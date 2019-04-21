#include "Socket/UDPsocket.hpp"

#include "Tools.hpp"

namespace simpleNET
{
    UDPsocket::UDPsocket(bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP))
    {
    }

    UDPsocket::UDPsocket(SOCKET socketID)
        : SimpleSocket(socketID)
    {
    }

    UDPsocket::UDPsocket(const std::string &ipAddress, int port, bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP))
    {
        Bind(ipAddress, port, ipV6 ? AF_INET6 : AF_INET);
    }

    UDPsocket::UDPsocket(int port, bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP))
    {
        BindAll(port, ipV6 ? AF_INET6 : AF_INET);
    }

} // namespace simpleNET
