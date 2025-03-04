#pragma once
#include <WinSock2.h>
#include <vector>
#include <thread>
#include "ClientContext.h"
#include "PlaneDataStorage.h"

#define PORT 8080

/*
https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports
*/


class Server
{
public:

	Server(int port = PORT);
	~Server();

    void MainThread();


private:
	int port;
	SOCKET serverSocket;
	WSAData wsaData;
	HANDLE hIOCP;
	PlaneDataStorage pds;
	int nextClientId;

	void WorkerThread();
};

