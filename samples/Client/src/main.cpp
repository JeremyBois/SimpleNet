#include "simpleNET.hpp"
#include <iostream>

#include <cereal/archives/binary.hpp>
#include <sstream>



struct MyClass
{
    public:
        int x, y, z;
    // This method lets cereal know which data members to serialize
    template<class Archive>
    void serialize(Archive & archive)
    {
      archive( x, y, z ); // serialize things by passing them to the archive
    }
};

void TestCereal()
{
    std::cout << "\nSTART Cereal Test" << std::endl;
    // Test Cereal
    std::stringstream ss; // any stream can be used

    // RAII --> Data guaranteed  to flush on destruction --> create scope to force destruction
    {
        cereal::BinaryOutputArchive oarchive(ss); // Create an output archive

        MyClass m1, m2, m3;
        m1.x = 23;
        m2.y = 444;

        oarchive(m1, m2, m3); // Write the data to the archive
    } // archive goes out of scope, ensuring all contents are flushed

    {
        cereal::BinaryInputArchive iarchive(ss); // Create an input archive

        MyClass m1, m2, m3;
        iarchive(m1, m2, m3); // Read the data from the archive

        std::cout << m1.x << " -- " << m2.y << std::endl;
    }

    std::cout << "END Cereal Test \n" << std::endl;
}


namespace Net = simpleNET;


int main(int argc, char **argv)
{
    // Test Cereal
    TestCereal();

    // Init
    Net::SimpleNetInit();

    // Test purpose
    std::cout << "I'm the CLIENT" << std::endl;


    // Connect to local server (server must be start first)
    Net::ClientSocket clientSoc;
    if (clientSoc.Connect("127.0.0.1", 7777))
    {
        // Init buffers
        char buffer[2048] = {0};
        char bufferSend[2048] = "Bonjour Server.";

        // Using Protocol
        Net::TextProtocol myProto(clientSoc);

        // Send first connection
        myProto.Send(buffer);
        printf("Send: %s \n", buffer);

        // Mark socket as non-blocking
        clientSoc.MarkAsNonBlocking();

        // Will loop now
        while(true)
        {

            // Data to send / receive
            // Receive using Text protocol
            Net::TextProtocol myProto(clientSoc);
            int result = myProto.Receive(buffer);

            if (result == 0 || result == SOCKET_ERROR)
            {
                if (Net::Tools::GetLastErrorCodeID() != 10035)
                {
                    // Client deconnected or error occurs
                    printf("Bye Bye");
                    break;
                }
            }
            else
            {
                printf("Send: %s with result %d\n", buffer, result);
            }

        }
    }

    // Clean
    Net::SimpleNetCleanup();

    return 0;
}
