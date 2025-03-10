#pragma once
#pragma comment(lib, "Ws2_32.lib")
#include <WinSock2.h>
#include <vector>
#include <thread>
#include "ClientContext.h"
#include "PlaneDataStorage.h"
#include "Logger.h"

#define PORT 8080
#define BUFFER_SIZE 512

/*
https://learn.microsoft.com/en-us/windows/win32/fileio/i-o-completion-ports
*/

class Server
{
public:
	Server(int port = PORT, const std::string& dbConnString = );
	~Server();

	void MainThread();

private:
	int port;
	SOCKET serverSocket;
	WSAData wsaData;
	HANDLE hIOCP;
	PlaneDataStorage pds;
	Logger logger;

	void WorkerThread();
	float calculateFuelConsumption(TelemetryData previousTelemetryData, TelemetryData newTelemetryData);
};