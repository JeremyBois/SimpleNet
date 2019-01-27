#ifndef __TOOLS__HPP
#define __TOOLS__HPP

#include "simpleNet_macro.hpp" // Needed for macro definition


namespace simpleNET
{

    // Singleton class which defines utilitary methods
    class SIMPLENET_LOCAL Tools
    {
        private:
            Tools() = delete;
            ~Tools() = delete;

        public:
            static int         GetLastErrorCodeID();
            static std::string GetLastErrorMsg(int msgID);
            static void        PrintLastErrorMsg();
    };


} // namespace simpleNET

#endif
