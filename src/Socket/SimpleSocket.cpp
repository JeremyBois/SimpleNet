#include "Socket/SimpleSocket.hpp"

#include "Tools.hpp"

namespace simpleNET
{
    SimpleSocket::SimpleSocket()
        : AbstractSocket(INVALID_SOCKET)
    {
    }

    SimpleSocket::SimpleSocket(SOCKET socketID)
        : AbstractSocket(socketID)
    {
    }

    int SimpleSocket::Send(char* buffer, int bufferSize)
    {
        return SendImplementation(buffer, bufferSize, nullptr, 0, 0);
    }


    int SimpleSocket::Send(char* buffer, int bufferSize,
                           sockaddr* socAdress, socklen_t socAdressLength)
    {
        return SendImplementation(buffer, bufferSize, socAdress, socAdressLength, 0);
    }


    int SimpleSocket::Receive(char* buffer, int bufferSize)
    {
        return ReceiveImplementation(buffer, bufferSize, nullptr, nullptr, 0);
    }


    int SimpleSocket::Receive(char* buffer, int bufferSize,
                        sockaddr* socAdress, socklen_t* socAdressLength)
    {
        return ReceiveImplementation(buffer, bufferSize, socAdress, socAdressLength, 0);
    }

    int SimpleSocket::SendImplementation(char *buffer, int bufferSize,
                                      sockaddr *socAdress, socklen_t socAdressLength,
                                      int flags)
    {
        int dataSent = sendto(GetID(), buffer, bufferSize,
                              flags, socAdress, socAdressLength);

        if (dataSent == SOCKET_ERROR)
        {
            fprintf(stderr, "Send operation failed with error # %ld\n",
                    Tools::GetLastErrorCodeID());
        }

        return dataSent;
    }

    int SimpleSocket::ReceiveImplementation(char *buffer, int bufferSize,
                                         sockaddr *socAdress, socklen_t *socAdressLength,
                                         int flags)
    {
        int dataRead = recvfrom(GetID(), buffer, bufferSize,
                                flags, socAdress, socAdressLength);
        if (dataRead == SOCKET_ERROR)
        {
            int resultCode = Tools::GetLastErrorCodeID();
            fprintf(stderr, "Receive operation failed with error # %ld\n",
                    Tools::GetLastErrorCodeID());
        }

        return dataRead;
    }

} // namespace simpleNET
