#ifndef __TCP_SOCKET__HPP
#define __TCP_SOCKET__HPP

#include "Socket/SimpleSocket.hpp"

namespace simpleNET
{

    /// TCP socket wrapper
    class SIMPLENET_API TCPsocket : public SimpleSocket
    {
    public:
        /// Construct a TCP socket without any binding
        TCPsocket(bool ipV6 = false);
        /// Construct a TCP socket bind to this ipAddress and port
        TCPsocket(const std::string &ipAddress, int port, bool ipV6 = false);
        /// Construct a TCP socket bind to all interface and port
        TCPsocket(int port, bool ipV6 = false);

    protected:
        TCPsocket(SOCKET socketID);

    public:
        bool      Connect(std::string const &ipAddress, int port, bool ipV6 = false);
        bool      Listen();
        TCPsocket Accept();

    protected:
        virtual int SendImplementation(char *buffer, int bufferSize,
                                       sockaddr *socAdress, socklen_t socAdressLength,
                                       int flags = 0) override;
        virtual int ReceiveImplementation(char *buffer, int bufferSize,
                                          sockaddr *socAdress, socklen_t *socAdressLength,
                                          int flags = 0) override;

    protected:
        bool _Connect(const addrinfo *paddrInfos, addrinfo &usedInfo);

    };

} // namespace simpleNET

#endif
