#include "Protocol/AbstractProtocol.hpp"


namespace simpleNET
{
    AbstractProtocol::AbstractProtocol(SimpleSocket& socket)
        : _attachedSocket(socket)
    {
    }

    // Move semantics
    AbstractProtocol::AbstractProtocol(AbstractProtocol &&otherProtocol) noexcept
        : _attachedSocket(otherProtocol._attachedSocket)
    {
    }

    AbstractProtocol &AbstractProtocol ::operator=(AbstractProtocol &&otherProtocol) noexcept
    {
        otherProtocol.swap(*this);
        return *this;
    }

    void AbstractProtocol::swap(AbstractProtocol &otherProtocol) noexcept
    {
        std::swap(_attachedSocket, otherProtocol._attachedSocket);
    }


    int AbstractProtocol::Send(char* buffer)
    {
        return _attachedSocket.Send(buffer, strlen(buffer), nullptr, 0);
    }


    int AbstractProtocol::Send(char* buffer,
                               sockaddr* socAdress, socklen_t socAdressLength)
    {
        return _attachedSocket.Send(buffer, strlen(buffer), socAdress, socAdressLength);
    }


    int AbstractProtocol::Receive(char* buffer)
    {
        return _attachedSocket.Receive(buffer, strlen(buffer), nullptr, nullptr);
    }


    int AbstractProtocol::Receive(char* buffer,
                                  sockaddr* socAdress, socklen_t* socAdressLength)
    {
        return _attachedSocket.Receive(buffer, strlen(buffer), socAdress, socAdressLength);
    }

} // namespace simpleNET
