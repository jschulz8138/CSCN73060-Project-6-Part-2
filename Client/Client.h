#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../Shared/GeneratePacket.h"
#include "../Shared/SendPacket.h"
#include "../Shared/EndPacket.h"
#include "../Shared/AckPacket.h"
#include "../Shared/PacketFactory.h"

#define SERVER_PORT 8080
#define BUFFER_SIZE 512

class Client {
public:
    Client();
    ~Client();

    bool Connect(const char*);
    void SendPacket(std::unique_ptr<Packet>& packet);
    std::unique_ptr<Packet> ReceivePacket();
    void Run(const char*);

private:
    SOCKET clientSocket;
};