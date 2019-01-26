#include "simpleNET.hpp"

namespace Net = simpleNET;


int main(int argc, char **argv)
{
    // Init
    Net::SimpleNetInit();

    // Test purpose
    cout << "I'm the CLIENT" << endl;


    // Test lib import
    Net::Socket aSocket;
    Net::Client aClient;
    Net::Server aServer;

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
