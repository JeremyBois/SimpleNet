#ifndef __SIMPLENETMACRO__HPP
#define __SIMPLENETMACRO__HPP

// More at
// https://cmake.org/Wiki/BuildingWinDLL
// https://gcc.gnu.org/wiki/Visibility

// Generic helper definitions for shared library support
#if defined _WIN32 || defined __CYGWIN__
    #if defined __MINGW32__
        // GCC with MINGW
        #define SIMPLENET_HELPER_DLL_IMPORT __attribute__((visibility("default")))
        #define SIMPLENET_HELPER_DLL_EXPORT __attribute__((visibility("default")))
        #define SIMPLENET_HELPER_DLL_LOCAL __attribute__((visibility("hidden")))
        #define BUILD_WITH "MINGW"
    #else
    // Microsoft
        #define SIMPLENET_HELPER_DLL_IMPORT __declspec(dllimport)
        #define SIMPLENET_HELPER_DLL_EXPORT __declspec(dllexport)
        #define SIMPLENET_HELPER_DLL_LOCAL
        #define BUILD_WITH "MSVC"
        #endif
    #else
    #if defined __GNUC__ >= 4
        // GCC
        #define SIMPLENET_HELPER_DLL_IMPORT __attribute__((visibility("default")))
        #define SIMPLENET_HELPER_DLL_EXPORT __attribute__((visibility("default")))
        #define SIMPLENET_HELPER_DLL_LOCAL __attribute__((visibility("hidden")))
        #define BUILD_WITH "GCC"
    #else
        // GCC does not support __attribute__ before version 4.
        #define SIMPLENET_HELPER_DLL_IMPORT
        #define SIMPLENET_HELPER_DLL_EXPORT
        #define SIMPLENET_HELPER_DLL_LOCAL
    #endif
#endif

// Now we use the generic helper definitions above to define SIMPLENET_API and SIMPLENET_LOCAL.
// SIMPLENET_API is used for the public API symbols. It either DLL imports or DLL exports (or does nothing for static build)
// SIMPLENET_LOCAL is used for non-api symbols.
#ifdef SIMPLENET_SHARED         // Defined if SIMPLENET is compiled as a DLL
    #ifdef SIMPLENET_SHARED_EXPORTS // Defined if we are building the SIMPLENET DLL (instead of using it)
        #define SIMPLENET_API SIMPLENET_HELPER_DLL_EXPORT
    #else
        #define SIMPLENET_API SIMPLENET_HELPER_DLL_IMPORT
    #endif // SIMPLENET_SHARED_EXPORTS
    #define SIMPLENET_LOCAL SIMPLENET_HELPER_DLL_LOCAL
#else // SIMPLENET_SHARED is not defined: this means SIMPLENET is a STATIC lib.
    #define SIMPLENET_API
    #define SIMPLENET_LOCAL
#endif // SIMPLENET_SHARED


// STL
#include <iostream>

// Network
#if defined __unix__
    #include <sys/types.h>
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <arpa/inet.h>
    #include <unistd.h>
    #include <netdb.h>

    // Match windows constant and typedef definition
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
    #define closesocket(s) close(s)
    typedef int SOCKET;
    typedef struct sockaddr_in SOCKADDR_IN;
    typedef struct sockaddr SOCKADDR;
    typedef struct in_addr IN_ADDR;
#elif defined _WIN32
    #include <winsock2.h>
    #include "Ws2tcpip.h"

    #pragma comment(lib, "Ws2_32.lib")
#else
    #error Not defined for this platform
#endif


#endif
