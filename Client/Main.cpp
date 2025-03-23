#include "Client.h"

#define IP_INDEX 1

int main(int argc, char* argv[]) {
    
    // Make sure only 2 arguments are in command line
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <IP_ADDRESS>\n";
        return 1;
    }

    try {
        Client client;
        client.Run(argv[IP_INDEX]);
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}