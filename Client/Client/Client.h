#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <ws2tcpip.h>
#include <iostream>
#include "../../Shared/Packet.h"

#define SERVER_IP "127.0.0.1"
#define SERVER_PORT 8080
#define BUFFER_SIZE 512

class Client {
public:
    Client();
    ~Client();

    bool Connect();
    void SendPacket(const Packet& packet);
    Packet ReceivePacket();
    void Run();

private:
    SOCKET clientSocket;
};