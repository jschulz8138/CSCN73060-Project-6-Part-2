#pragma once
#include <WinSock2.h>
#include <vector>
#include <thread>

#define MAXTHREADS 10
#define BUFFER_SIZE 512


class Server
{
public:

	Server(int MaxThreads = MAXTHREADS);

	~Server();


private:
    struct PerClientContext {
        SOCKET clientSocket;
        OVERLAPPED overlapped;
        WSABUF wsaBuf;
        char buffer[BUFFER_SIZE];
        DWORD bytesReceived;
    };

    SOCKET serverSocket;
    HANDLE hIOCP;
    std::vector<std::thread> workers;


};

