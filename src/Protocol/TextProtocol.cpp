#include "Protocol/TextProtocol.hpp"


namespace simpleNET
{
    /// Same data but different representation
    union MsgSize
    {
        size_t size;
        char buff[sizeof(size_t)];
    };


    TextProtocol::TextProtocol(SimpleSocket& socket)
        : AbstractProtocol(socket)
    {
    }


    int TextProtocol::Send(char* buffer)
    {
        int totalSend = 0;

        // Get size
        MsgSize msgSize;
        msgSize.size = strlen(buffer);

        // Send size first
        totalSend += _attachedSocket.Send(msgSize.buff, sizeof(size_t), nullptr, 0);

        // Send message
        totalSend += _attachedSocket.Send(buffer, msgSize.size, nullptr, 0);


        return totalSend;
    }


    int TextProtocol::Send(char* buffer,
                               sockaddr* socAdress, socklen_t socAdressLength)
    {
        return _attachedSocket.Send(buffer, strlen(buffer), socAdress, socAdressLength);
    }


    int TextProtocol::Receive(char* buffer)
    {
        int stepRecv = 0;
        int recv = 0;

        // Get size
        char sizeBuffer[sizeof(size_t)];
        stepRecv = _attachedSocket.Receive(sizeBuffer, sizeof(size_t), nullptr, 0);
        recv += stepRecv;

        // Get text and null terminate it
        int dataSize = *(int*)sizeBuffer;
        stepRecv = _attachedSocket.Receive(buffer, dataSize, nullptr, nullptr);
        buffer[stepRecv] = '\0';
        recv += stepRecv;

        return recv;
    }


    int TextProtocol::Receive(char* buffer,
                                  sockaddr* socAdress, socklen_t* socAdressLength)
    {
        return _attachedSocket.Receive(buffer, strlen(buffer), socAdress, socAdressLength);
    }
}
