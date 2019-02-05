#include "Socket/AbstractSocket.hpp"

#include <string>
#include <utility>
#include "Tools.hpp"


namespace simpleNET
{
    AbstractSocket::AbstractSocket(SOCKET socketID)
        : _socketID(socketID), _iblockingMode(0)
    {
        // Default to blocking mode
    }

    AbstractSocket::AbstractSocket()
        : _socketID(INVALID_SOCKET), _iblockingMode(0)
    {
        // Default to blocking mode
    }

    AbstractSocket::~AbstractSocket()
    {
        if (_socketID != INVALID_SOCKET)
        {
            Close();
        }
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
        std::swap(_iblockingMode, otherSocket._iblockingMode);
    }

    // Methods
    void AbstractSocket::Init()
    {
    }

    void AbstractSocket::Close()
    {
        // Call close on unix systems
        closesocket(_socketID);
        _socketID = INVALID_SOCKET;
    }

    bool AbstractSocket::SetAsBlocking(bool shouldBlock)
    {
        // First check for previous mode
        u_long newMode = shouldBlock ? 0 : 1;

        if (_iblockingMode == newMode)
        {
            if (_iblockingMode == 0)
                fprintf(stderr, "Socket already set as blocking. Nothing done.\n");
            else
                fprintf(stderr, "Socket already set as non-blocking. Nothing done.\n");
            return false;
        }

        // Start updating state
        int iResult = SOCKET_ERROR;

    #ifdef _WIN32
        // https://docs.microsoft.com/en-us/windows/desktop/api/winsock/nf-winsock-ioctlsocket
        iResult = ioctlsocket(_socketID, FIONBIO, &newMode);
    #else
        // Update bitmask to keep other flags
        const int flags = fcntl(_socketID, F_GETFL, 0);
        if (flags != SOCKET_ERROR)
        {
            iResult = fcntl(_socketID, F_SETFL, shouldBlock ? flags ^ O_NONBLOCK : flags | O_NONBLOCK);
        }
    #endif

        // @TODO Handle error messages
        if (iResult == SOCKET_ERROR)
        {
            fprintf(stderr, "Socket blocking state cannot be set due to error # %ld\n",
                    Tools::GetLastErrorCodeID());
            return false;
        }
        else
        {
            _iblockingMode = newMode;
            return true;
        }

    }

    bool AbstractSocket::MarkAsBlocking()
    {
        return SetAsBlocking(true);
    }

    bool AbstractSocket::MarkAsNonBlocking()
    {
        return SetAsBlocking(false);
    }

    bool AbstractSocket::IsBlocking()
    {
        return _iblockingMode == 0;
    }

    int AbstractSocket::Select(int numfds, fd_set *readfds, fd_set *writefds,
                               fd_set *exceptfds, timeval *timeout)
    {
        int iResult = select(numfds, readfds, writefds, exceptfds, timeout);
        if (iResult == SOCKET_ERROR)
        {
            fprintf(stderr, "Select failed due to error  # %ld\n",
                    Tools::GetLastErrorCodeID());
        }

        return iResult;
    }

} // namespace simpleNET
