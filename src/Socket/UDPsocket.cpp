#include "Socket/UDPsocket.hpp"

#include "Tools.hpp"

namespace simpleNET
{
    UDPsocket::UDPsocket(bool ipV6)
        : SimpleSocket(socket(ipV6 ? AF_INET6 : AF_INET, SOCK_DGRAM, IPPROTO_UDP))
    {
        // Unlike TCP socket, UDP socket should be valid at creation
    }

    UDPsocket::UDPsocket(const std::string &ipAddress, int port, bool ipV6)
    {
        Bind(ipAddress, port, ipV6 ? AF_INET6 : AF_INET);
    }

} // namespace simpleNET
