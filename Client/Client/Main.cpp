#include "Client.h"
int main() {
    try {
        Client client;
        client.Run();
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}