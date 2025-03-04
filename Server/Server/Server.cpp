#include "Server.h"
#include "../../Shared/Packet.h"

Server::Server(int port)
{
	this->port = port;
	this->nextClientId = 1;

	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
		throw std::runtime_error("Failed to start up WSA.");

	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == INVALID_SOCKET)
		throw std::runtime_error("Failed to create socket.");

	sockaddr_in serverAddr = {};
	serverAddr.sin_family = AF_INET;
	serverAddr.sin_port = htons(this->port);
	serverAddr.sin_addr.s_addr = INADDR_ANY;

	if (bind(this->serverSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
		throw std::runtime_error("Failed to bind to port.");

	if (listen(this->serverSocket, SOMAXCONN) != 0)
		throw std::runtime_error("Failed to listen.");

	this->hIOCP = CreateIoCompletionPort(INVALID_HANDLE_VALUE, NULL, 0, std::thread::hardware_concurrency());
	if (this->hIOCP == NULL)
		throw std::runtime_error("Failed to create IO Completion Port.");

	if (CreateIoCompletionPort((HANDLE)this->serverSocket, this->hIOCP, 0, 0) == NULL)
		throw std::runtime_error("Failed to associate server socket with IOCP.");
}

Server::~Server()
{
	if(closesocket(this->serverSocket) != 0)
		throw std::runtime_error("Failed to close socket.");

	if (this->hIOCP != NULL && !CloseHandle(this->hIOCP))
		throw std::runtime_error("Failed to close IO Completion Port.");

	if(WSACleanup() != 0)
		throw std::runtime_error("Failed to clean up WSA.");
}

void Server::MainThread()
{
	//Create my workers 
	for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
		std::thread worker(&Server::WorkerThread, this);
		worker.detach();
	}

	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);

	while (true) {
		SOCKET clientSocket = accept(this->serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == INVALID_SOCKET)
		{
			//Log this and begin the next client connection.
			//"Failed to setup client socket."
			continue;
		}

		int clientId = nextClientId++;
		ClientContext* clientContext = new ClientContext(clientSocket, clientId);

		if (CreateIoCompletionPort((HANDLE)clientSocket, this->hIOCP, (ULONG_PTR)clientContext, 0) == NULL)
		{
			//Log this and begin the next client connection.
			//"Failed to associate client socket with IOCP."
			continue;
		}

		//We can use trash Telemetry Data here
		//FIX TELEMETRY DATA HERE
		Packet idPacket(ProtocolFlag::GENERATEID, clientId, TelemetryData());
		std::vector<char> serializedPacket = idPacket.SerializeData();

		DWORD flags = 0;
		DWORD bytesRecieved = 0;
		int result = WSARecv(clientSocket, &clientContext->wsaBuf, 1, &bytesRecieved, &flags, &clientContext->overlapped, nullptr);

		if (result == SOCKET_ERROR && WSAGetLastError() != WSA_IO_PENDING)
		{
			//Log this and begin the next client connection.
			//WSARecv has failed. Clean up the client connection and accept a new connection.
			delete clientContext;
			closesocket(clientSocket);
		}
	}
}

void Server::WorkerThread()
{

}
