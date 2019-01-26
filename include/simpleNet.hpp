#ifndef __SIMPLENET__HPP
#define __SIMPLENET__HPP

// Network
#include "Client.hpp"
#include "Server.hpp"
#include "Socket.hpp"


// Windows stuff needed for socket initialization
bool SimpleNetInit()
{
#if defined _WIN32 && !defined __MINGW32__
    cout << "Build on : " << TEST_SHARED_WINDOWS << endl;
    WSAData data;
    if (WSAStartup(MAKEWORD(2, 2), &data) != 0)
    {
        WSACleanup();
        return false;
    }
    cout << "Initialization done" << endl;
    return true;
#else
    cout << "Build on : " << TEST_SHARED_WINDOWS << endl;
    cout << "Initialization done automatically" << endl;
    return true;
#endif
}

void SimpleNetCleanup()
{
#if defined _WIN32 && !defined __MINGW32__
        WSACleanup();
#endif

    // Automatically handle on unix systems
}

#endif
