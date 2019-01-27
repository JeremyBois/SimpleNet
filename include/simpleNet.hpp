#ifndef __SIMPLENET__HPP
#define __SIMPLENET__HPP

// Network
#include "Client.hpp"
#include "Server.hpp"

// Socket
#include "Socket/SimpleSocket.hpp"
#include "Socket/ClientSocket.hpp"
#include "Socket/ServerSocket.hpp"


namespace simpleNET
{
    // Windows stuff needed for socket initialization
    bool SimpleNetInit()
    {
    #if defined _WIN32
        std::cout << "Build using : " << BUILD_WITH << std::endl;
        WSAData data;
        if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
        {
            WSACleanup();
            return false;
        }
        std::cout << "Initialization done" << std::endl;
        return true;
    #else
        std::cout << "Build using : " << BUILD_WITH << std::endl;
        std::cout << "Initialization done automatically" << std::endl;
        return true;
    #endif
    }

    void SimpleNetCleanup()
    {
    #if defined _WIN32
        WSACleanup();
    #endif

        // Automatically handle on unix systems
    }
}


#endif
