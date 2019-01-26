#ifndef __SERVER__HPP
#define __SERVER__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition

namespace simpleNET
{

    // Server interface
    class SIMPLENET_API Server
    {
        public:
            Server() = default;
            virtual ~Server() = default;

            // Disable copy
            Server(const Server& otherServer) = delete;
            Server &operator=(Server const &otherServer) = delete;
    };

} // namespace simpleNET

#endif
