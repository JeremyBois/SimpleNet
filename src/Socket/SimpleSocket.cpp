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


    int SimpleSocket::SendImplementation(char* buffer, int bufferSize,
                                         sockaddr* socAdress, socklen_t socAdressLength,
                                         int flags)
    {
        // Message not always send in one block
        int dataSent = 0;
        int remainingCount = bufferSize;

        while(dataSent < bufferSize)
        {
            // Send in TCP == SendTo without socAdress
            int nbCharSent = sendto(GetID(), buffer + dataSent, remainingCount,
                                    flags, socAdress, socAdressLength);
            // int nbCharSent = send(GetID(), buffer + dataSent, remainingCount, flags);
            // printf("-- %d -- \n", nbCharSent);

            if (nbCharSent == SOCKET_ERROR)
            {
                // @TODO Handle error code
                fprintf(stderr, "Send operation failed (%d / %d sent) with error # %ld\n",
                        dataSent, bufferSize, Tools::GetLastErrorCodeID());
                break;
            }
            else
            {
                dataSent += nbCharSent;
                remainingCount -= nbCharSent;
            }
        }

        return dataSent;

    }

    int SimpleSocket::ReceiveImplementation(char* buffer, int bufferSize,
                                            sockaddr* socAdress, socklen_t* socAdressLength,
                                            int flags)
    {
        int dataRead = 0;
        int dataMax = bufferSize - 1;
        int remainingCount = bufferSize;

        while(dataRead < dataMax)
        {
            // Recv in TCP == RecvFrom without socAdress
            int nbCharRecv = recvfrom(GetID(), buffer + dataRead, remainingCount,
                                      flags, socAdress, socAdressLength);
            // int nbCharRecv = recv(GetID(), buffer + dataRead, remainingCount, flags);
            // printf("-- %d -- \n", nbCharRecv);

            if (nbCharRecv == SOCKET_ERROR)
            {
                // @TODO Handle error code
                fprintf(stderr, "Receive operation failed (%d / %d received) with error # %ld\n",
                        dataRead, bufferSize, Tools::GetLastErrorCodeID());
                break;
            }
            else if (nbCharRecv == 0)
            {
                // Message over
                break;
            }
            else
            {
                dataRead += nbCharRecv;
                remainingCount -= nbCharRecv;
            }
        }

        return dataRead;
    }

} // namespace simpleNET
