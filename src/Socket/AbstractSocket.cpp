#include "Socket/AbstractSocket.hpp"

#include <string>
#include <utility>


namespace simpleNET
{
    AbstractSocket::AbstractSocket(SOCKET socketID)
        : _socketID(socketID)
    {
    }

    AbstractSocket::~AbstractSocket()
    {
        Close();
    }

    // Move semantics
    AbstractSocket::AbstractSocket(AbstractSocket &&otherSocket) noexcept
    {
        otherSocket.swap(*this);
    }

    AbstractSocket &AbstractSocket ::operator=(AbstractSocket &&otherSocket) noexcept
    {
        otherSocket.swap(*this);
        return *this;
    }

    void AbstractSocket::swap(AbstractSocket &otherSocket) noexcept
    {
        std::swap(_socketID, otherSocket._socketID);
    }

    // Methods
    void AbstractSocket::Init()
    {
    }

    void AbstractSocket::Close()
    {
        // Call close on unix systems
        closesocket(_socketID);
    }

} // namespace simpleNET
