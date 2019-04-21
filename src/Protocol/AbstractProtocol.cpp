#include "Protocol/AbstractProtocol.hpp"


namespace simpleNET
{
    AbstractProtocol::AbstractProtocol(SimpleSocket* socket)
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

} // namespace simpleNET
