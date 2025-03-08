#include "Server.h"
#include "../Shared/GeneratePacket.h"
#include "../Shared/SendPacket.h"
#include "../Shared/EndPacket.h"
#include "../Shared/AckPacket.h"
#include "../Shared/PacketFactory.h"

Server::Server(int port)
{
	this->port = port;

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
	if (closesocket(this->serverSocket) != 0)
		throw std::runtime_error("Failed to close socket.");

	if (this->hIOCP != NULL && !CloseHandle(this->hIOCP))
		throw std::runtime_error("Failed to close IO Completion Port.");

	if (WSACleanup() != 0)
		throw std::runtime_error("Failed to clean up WSA.");
}

//Bourgeoisie
void Server::MainThread()
{
	//Create my workers
	//-1 because the main thread is processing the entire time.
	std::vector<std::thread> workerThreads;
	for (int i = 0; i < std::thread::hardware_concurrency() - 1; ++i) {
		workerThreads.push_back(std::thread(&Server::WorkerThread, this));
	}

	sockaddr_in clientAddr;
	int clientAddrSize = sizeof(clientAddr);

	while (true) {
		//Make a new friend
		SOCKET clientSocket = accept(this->serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == INVALID_SOCKET) {
			this->logger.logMessage("Failed to setup client socket.");
			continue;
		}

		//Setup the client context struct.
		int clientId = this->pds.generateNewId();
		ClientContext* clientContext = new ClientContext();
		clientContext->clientSocket = clientSocket;
		clientContext->buffer.resize(BUFFER_SIZE);
		clientContext->wsaBuf.buf = clientContext->buffer.data();
		clientContext->wsaBuf.len = BUFFER_SIZE;
		ZeroMemory(&clientContext->overlapped, sizeof(OVERLAPPED));

		if (CreateIoCompletionPort((HANDLE)clientSocket, this->hIOCP, (ULONG_PTR)clientContext, 0) == NULL) {
			this->logger.logMessage("Failed to associate client socket with IOCP.");
			closesocket(clientSocket);
			delete clientContext;
			continue;
		}

		//We can use trash Telemetry Data here
		//FIX TELEMETRY DATA HERE
		Packet idPacket = PacketFactory::create(ProtocolFlag::GENERATEID, clientId);
		std::vector<char> serializedPacket = idPacket.SerializeData();

		if (send(clientSocket, serializedPacket.data(), serializedPacket.size(), 0) == SOCKET_ERROR) {
			this->logger.logMessage("Failed to send GENERATEID packet.");
			closesocket(clientSocket);
			delete clientContext;
			continue;
		}

		//Post the first recv request before moving on to the next client.
		//Remember, WSARecv is non blocking and it only collect the current cient
		DWORD flags = 0;
		if (WSARecv(clientSocket, &clientContext->wsaBuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr) == SOCKET_ERROR) {
			this->logger.logMessage("WSARecv failed on first read. Closing connection.");
			closesocket(clientSocket);
			delete clientContext;
			continue;
		}
	}

	//Join threads during shutdown
	for (std::thread& worker : workerThreads) {
		if (worker.joinable()){
			worker.join();
		}
	}
}

//Proletariat
void Server::WorkerThread()
{
	DWORD bytesTranferred;
	ClientContext* clientContext;
	OVERLAPPED* overlapped;

	while (true){
		if (!GetQueuedCompletionStatus(this->hIOCP, &bytesTranferred, (PULONG_PTR)&clientContext, &overlapped, INFINITE)) {
			this->logger.logMessage("Failure while dequeuing IOCP. Cleaning up the client connection.");
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		if(bytesTranferred == 0){
			this->logger.logMessage("Client has disconnected. Cleaning up the client connection.");
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		//Process the packet
		Packet receivedPacket = PacketFactory::create(clientContext->buffer);
		if (!receivedPacket.validateTelemetryData()) {
			this->logger.logMessage("Received packet has not been deserialized properly");
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		switch (receivedPacket.getFlag()) {
		case ProtocolFlag::SENDDATA:
			this->pds.storeTelemetryData(receivedPacket.getId(), receivedPacket.getTelemetryData());
			break;
		case ProtocolFlag::ENDCOMMUNICATION:
			this->pds.storeAverage(receivedPacket.getId());
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		default:
			this->logger.logMessage("Received packet with an unknown flag.");
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		Packet ackPacket = PacketFactory::create(ProtocolFlag::ACK);
		std::vector<char> serializedAck = ackPacket.SerializeData();
		send(clientContext->clientSocket, serializedAck.data(), serializedAck.size(), 0);

		ZeroMemory(&clientContext->overlapped, sizeof(OVERLAPPED));
		DWORD flags = 0;
		if (WSARecv(clientContext->clientSocket, &clientContext->wsaBuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr) == SOCKET_ERROR) {
			this->logger.logMessage("WSARecv failed. Closing connection.");
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}
	}
}
