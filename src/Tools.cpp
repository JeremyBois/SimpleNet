#include "Tools.hpp"

#include <string>


namespace simpleNET
{

    int Tools::GetLastErrorCodeID()
    {
        #if defined _WIN32
            // Assume Windows system
            return WSAGetLastError();
        #else
            // Assume Unix system
            return errno;
        #endif
    }

    std::string Tools::GetLastErrorMsg(int msgID)
    {
        return "Error";
    }

    void Tools::PrintLastErrorMsg()
    {

    }
} // namespace simpleNET
