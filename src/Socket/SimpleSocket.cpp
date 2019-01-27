#include "Socket/SimpleSocket.hpp"


namespace simpleNET
{
    SimpleSocket::SimpleSocket(SOCKET socketID)
        : AbstractSocket(socketID)
    {
    }

    bool SimpleSocket::Send()
    {
        return false;
    }

    bool SimpleSocket::Receive()
    {
        return false;
    }

} // namespace simpleNET
