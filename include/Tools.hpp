#ifndef __TOOLS__HPP
#define __TOOLS__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition

// @TODO Implement conversion from message ID to string

namespace simpleNET
{

    // Singleton class which defines utilitary methods
    class SIMPLENET_LOCAL Tools
    {
        private:
            Tools() = delete;
            ~Tools() = delete;

        public:
            /// Get last error code identifier
            static int         GetLastErrorCodeID();

            // /// Convert error message ID to corresponding string message
            // static std::string GetLastErrorMsg(int msgID);

            // /// Display last error message
            // static void        PrintLastErrorMsg();


            /// Iterate over adress information and display them
            static void DisplayAdresses(const addrinfo *pinfo);

            /// Find TCP adresses
            static int GetTCPAdresses(const std::string &ipAdress, int port,
                                      addrinfo *&infosContainer);

            /// Find UDP adresses
            static int GetUDPAdresses(const std::string &ipAdress, int port,
                                      addrinfo *&infosContainer);

            /// Retrieve adress service from socket adress (unsafe)
            static unsigned short GetSocketPort(const sockaddr *psockAdress);

            /// Retrieve adress name from socket adress (unsafe)
            static void*          GetSocketAdress(const sockaddr *psockAdress);

            /// Retrieve informations from socket adress (safe)
            static int GetAdressAndService(const sockaddr *psockAdress,
                                           size_t sizeStruct,
                                           std::string &adress,
                                           std::string &service);
    };


} // namespace simpleNET

#endif
