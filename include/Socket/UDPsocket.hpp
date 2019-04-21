#ifndef __UDP_SOCKET__HPP
#define __UDP_SOCKET__HPP

#include "Socket/SimpleSocket.hpp"

namespace simpleNET
{

    /// UDP socket wrapper
    class SIMPLENET_API UDPsocket : public SimpleSocket
    {
    public:
        /// Construct a UDP socket
        UDPsocket(bool ipV6 = false);
        /// Construct a UDP socket bind to this ipAddress and port
        UDPsocket(const std::string &ipAddress, int port, bool ipV6 = false);
        /// Construct a UDP socket bind to all interface and port
        UDPsocket(int port, bool ipV6 = false);

    protected:
        UDPsocket(SOCKET socketID);


        // @TODO Add Helper to sento and receivefrom
    };

} // namespace simpleNET

#endif
