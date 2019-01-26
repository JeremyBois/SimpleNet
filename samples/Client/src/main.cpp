#include "simpleNET.hpp"

namespace Net = simpleNET;


int main(int argc, char **argv)
{
    // Test purpose
    cout << "I'm the CLIENT" << endl;


    #if defined _WIN32
        // Test defined flags
        cout << TEST_SHARED_WINDOWS << endl;
    #endif


    // Test lib import
    Net::Socket aSocket;
    Net::Client aClient;
    Net::Server aServer;


    return 0;
}
