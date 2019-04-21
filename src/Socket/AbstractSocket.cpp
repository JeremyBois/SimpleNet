#include "Socket/AbstractSocket.hpp"

#include <string>
#include <utility>
#include "Tools.hpp"


namespace simpleNET
{
    AbstractSocket::AbstractSocket(SOCKET socketID)
        : _socketID(socketID)
    {
        // Default to blocking mode
        if (_socketID != INVALID_SOCKET)
        {
            SetAsBlocking(true);
        }

    }

    AbstractSocket::AbstractSocket()
        : _socketID(INVALID_SOCKET)
    {
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
        if (_iblockingMode == 0)
        {
            fprintf(stderr, "Socket already set as blocking. Nothing done.\n");
            return true;
        }
        else
        {
            return SetAsBlocking(true);
        }
    }

    bool AbstractSocket::MarkAsNonBlocking()
    {
        if (_iblockingMode == 0)
        {
            fprintf(stderr, "Socket already set as non-blocking. Nothing done.\n");
            return true;
        }
        else
        {
            return SetAsBlocking(false);
        }
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

    void AbstractSocket::Bind(sockaddr_in &sockAdressV4)
    {
        // Link socket to communication port
        if (bind(GetID(), (sockaddr *)&sockAdressV4, sizeof(sockAdressV4)) != 0)
        {
            fprintf(stderr, "Server binding error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
        }
        else
        {
            // Find informations
            std::string name;
            std::string portUsed;
            int dwRetval = Tools::GetAdressAndService((sockaddr *)&sockAdressV4,
                                                      sizeof(sockAdressV4), name, portUsed);

            fprintf(stdout, "Server bind to port %s (%s)\n",
                    portUsed.c_str(), name.c_str());
        }
    }

    void AbstractSocket::Bind(sockaddr_in6 &sockAdressV6)
    {
        // Link socket to communication port
        if (bind(GetID(), (sockaddr *)&sockAdressV6, sizeof(sockAdressV6)) != 0)
        {
            fprintf(stderr, "Server binding error  (%d)\n",
                    Tools::GetLastErrorCodeID());
            Close();
        }
        else
        {
            // Find informations
            std::string name;
            std::string portUsed;
            int dwRetval = Tools::GetAdressAndService((sockaddr *)&sockAdressV6,
                                                      sizeof(sockAdressV6), name, portUsed);

            fprintf(stdout, "Server bind to port %s (%s)\n",
                    portUsed.c_str(), name.c_str());
        }
    }

    void AbstractSocket::Bind(const std::string &addr, unsigned short port, ADDRESS_FAMILY family)
    {
        // Construct IPv6 or IPv4
        if (family == AF_INET6)
        {
            sockaddr_in6 sockAdress;
            memset(&sockAdress, 0, sizeof(sockAdress)); // make sure the struct is empty

            sockAdress.sin6_family = family;
            // Specific adress
            inet_pton(PF_INET6, addr.c_str(), &sockAdress.sin6_addr);
            // Handle difference between little-endian and big-endian
            // https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
            sockAdress.sin6_port = htons(port);

            Bind(sockAdress);
        }
        else
        {
            sockaddr_in sockAdress;
            memset(&sockAdress, 0, sizeof(sockAdress)); // make sure the struct is empty

            sockAdress.sin_family = family;
            // Specific adress
            inet_pton(PF_INET, addr.c_str(), &sockAdress.sin_addr.s_addr);
            // Handle difference between little-endian and big-endian
            // https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
            sockAdress.sin_port = htons(port);

            Bind(sockAdress);
        }


    }

    void AbstractSocket::BindAll(unsigned short port, ADDRESS_FAMILY family)
    {
        // Construct IPv6 or IPv4
        if (family == AF_INET6)
        {
            sockaddr_in6 sockAdress;
            memset(&sockAdress, 0, sizeof(sockAdress)); // make sure the struct is empty
            sockAdress.sin6_family = AF_INET;
            // Bound to all interfaces (0.0.0.0)
            // https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
            sockAdress.sin6_addr = IN6ADDR_ANY_INIT;
            // Handle difference between little-endian and big-endian
            //https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
            sockAdress.sin6_port = htons(port);

            Bind(sockAdress);
        }
        else
        {
            sockaddr_in sockAdress;
            memset(&sockAdress, 0, sizeof(sockAdress)); // make sure the struct is empty
            sockAdress.sin_family = AF_INET;
            // Bound to all interfaces (0.0.0.0)
            // https://stackoverflow.com/questions/16508685/understanding-inaddr-any-for-socket-programming
            sockAdress.sin_addr.s_addr = htonl(INADDR_ANY);
            // Handle difference between little-endian and big-endian
            //https://stackoverflow.com/questions/19207745/htons-function-in-socket-programing#19209503
            sockAdress.sin_port = htons(port);

            Bind(sockAdress);
        }
    }

} // namespace simpleNET
