#ifndef __CLIENT__HPP
#define __CLIENT__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition

namespace simpleNET
{

    // Client interface
    class SIMPLENET_API Client
    {
        public:
            Client()          = default;
            virtual ~Client() = default;

            // Disable copy
            Client(const Client& otherClient)            = delete;
            Client &operator=(Client const &otherClient) = delete;
    };

} // namespace simpleNET


#endif
