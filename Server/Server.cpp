#include "Server.h"
#include "../Shared/GeneratePacket.h"
#include "../Shared/SendPacket.h"
#include "../Shared/EndPacket.h"
#include "../Shared/AckPacket.h"
#include "../Shared/PacketFactory.h"

Server::Server(const std::string& dbConnString, int port)
{
	this->port = port;
	this->dbConnectionString = dbConnString;

	if (WSAStartup(MAKEWORD(2, 2), &this->wsaData) != 0)
		throw std::runtime_error("Failed to start up WSA.");

	this->serverSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (this->serverSocket == INVALID_SOCKET)
		throw std::runtime_error("Failed to create socket.");

	std::cout << "Connecting to: " << this->dbConnectionString << std::endl;


	for (int i = 0; i < std::thread::hardware_concurrency(); ++i) {
		try {
			auto conn = std::make_unique<pqxx::connection>(dbConnectionString);
			if (!conn->is_open()) {
				throw std::runtime_error("Failed to open PostgreSQL connection.");
			}
			this->dbConnections.push_back(std::move(conn));
		}
		catch (const std::exception& ex) {
			std::cerr << "Database connection error: " << ex.what() << std::endl;
			throw std::runtime_error("Database connection error: " + std::string(ex.what()));
		}
	}


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
	shutdown(serverSocket, SD_SEND);
	if (closesocket(this->serverSocket) != 0)
		throw std::runtime_error("Failed to close socket.");

	if (this->hIOCP != NULL && !CloseHandle(this->hIOCP))
		throw std::runtime_error("Failed to close IO Completion Port.");

	if (WSACleanup() != 0)
		throw std::runtime_error("Failed to clean up WSA.");
}

//Bourgeoisie
//This guy is accepting new clients
void Server::MainThread()
{
	//Create my workers
	//-1 because the main thread is processing the entire time.
	std::vector<std::thread> workerThreads;
	for (int i = 0; i < std::thread::hardware_concurrency() - 1; ++i) {
		workerThreads.push_back(std::thread(&Server::WorkerThread, this, i));
	}


	std::cout << "Server is up and ready..." << std::endl;

	while (true) {
		//Make a new friend
		sockaddr_in clientAddr;
		int clientAddrSize = sizeof(clientAddr);

		std::cout << "Waiting for client..." << std::endl;
		SOCKET clientSocket = accept(this->serverSocket, (sockaddr*)&clientAddr, &clientAddrSize);
		if (clientSocket == INVALID_SOCKET) {
			this->logger.logMessage("Failed to setup client socket. " + std::to_string(WSAGetLastError()));
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
			shutdown(clientSocket, SD_SEND);
			closesocket(clientSocket);
			delete clientContext;
			continue;
		}

		//We can use trash Telemetry Data here
		//FIX TELEMETRY DATA HERE
		std::unique_ptr<Packet> idPacket;
		try {
			idPacket = PacketFactory::create(ProtocolFlag::GENERATEID, clientId);
		}
		catch (const std::exception& e) {
			this->logger.logMessage("Failure whiile using the packet factory for an ACK Packet.");
		}
		
		std::vector<char> serializedPacket = idPacket->SerializeData();

		if (send(clientSocket, serializedPacket.data(), serializedPacket.size(), 0) == SOCKET_ERROR) {
			this->logger.logMessage("Failed to send GENERATEID packet.");
			shutdown(clientSocket, SD_SEND);
			closesocket(clientSocket);
			delete clientContext;
			continue;
		}

		//Post the first recv request before moving on to the next client.
		//Remember, WSARecv is non blocking and it only collect the current cient
		DWORD flags = 0;
		if (WSARecv(clientSocket, &clientContext->wsaBuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr) == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING) {
				this->logger.logMessage("WSARecv failed on first read. Closing connection. Code:" + std::to_string(errorCode));
				shutdown(clientSocket, SD_SEND);
				closesocket(clientSocket);
				delete clientContext;
				continue;
			}
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
//Takes the packets and deals with them
//One packet at a time, not one client at a time.
void Server::WorkerThread(int threadIndex)
{
	auto& conn = *dbConnections[threadIndex];

	DWORD bytesTranferred;
	ClientContext* clientContext;
	OVERLAPPED* overlapped;

	std::unique_ptr<Packet> ackPacket;
	try {
		ackPacket = PacketFactory::create(ProtocolFlag::ACK);
	}
	catch (const std::exception& e) {
		this->logger.logMessage("Failure while using the packet factory for an ACK Packet.");
	}
	std::vector<char> serializedAck = ackPacket->SerializeData();

	while (true){
		if (!GetQueuedCompletionStatus(this->hIOCP, &bytesTranferred, (PULONG_PTR)&clientContext, &overlapped, INFINITE)) {
			this->logger.logMessage("Failure while dequeuing IOCP. Cleaning up the client connection.");
			shutdown(clientContext->clientSocket, SD_SEND);
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		if(bytesTranferred == 0){
			this->logger.logMessage("Client has disconnected. Cleaning up the client connection.");
			shutdown(clientContext->clientSocket, SD_SEND);
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		//Process the packet
		std::unique_ptr<Packet> receivedPacket;
		try {
			receivedPacket = PacketFactory::create(clientContext->buffer);
		}
		catch (const std::exception& e) {
			this->logger.logMessage("Failure while deserializing the recievedPacket");
		}

		//if (!receivedPacket->validateData()) {
		//	this->logger.logMessage("Received packet has not been deserialized properly");
		//  shutdown(clientContext->clientSocket, SD_SEND);
		//	closesocket(clientContext->clientSocket);
		//	delete clientContext;
		//	continue;
		//}

		switch (receivedPacket->getFlag()) {
		case ProtocolFlag::SENDDATA:
			//If this is the first communciation, initialize the prevTimeStamp and prevTelemetryData
			if (!clientContext->isPrevTelemetryDataInitialized) {
				clientContext->prevTelemetryData = receivedPacket->getTelemetryData();
				clientContext->isPrevTelemetryDataInitialized = true;
			}
			else {
				this->pds.storeFuelConsumption(conn, receivedPacket->getId(), receivedPacket->getTelemetryData().getFuel() - clientContext->prevTelemetryData.getFuel(), clientContext->prevTelemetryData.getFuelType());
				clientContext->prevTelemetryData = receivedPacket->getTelemetryData();
			}

			break;
		case ProtocolFlag::ENDCOMMUNICATION:
			this->pds.storeAverageFuelConsumption(conn, receivedPacket->getId());
			shutdown(clientContext->clientSocket, SD_SEND);
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		default:
			this->logger.logMessage("Received packet with an unknown flag.");
			shutdown(clientContext->clientSocket, SD_SEND);
			closesocket(clientContext->clientSocket);
			delete clientContext;
			continue;
		}

		send(clientContext->clientSocket, serializedAck.data(), serializedAck.size(), 0);

		ZeroMemory(&clientContext->overlapped, sizeof(OVERLAPPED));
		DWORD flags = 0;
		if (WSARecv(clientContext->clientSocket, &clientContext->wsaBuf, 1, nullptr, &flags, &clientContext->overlapped, nullptr) == SOCKET_ERROR) {
			int errorCode = WSAGetLastError();
			if (errorCode != WSA_IO_PENDING) {
				this->logger.logMessage("WSARecv failed. Closing connection.");
				shutdown(clientContext->clientSocket, SD_SEND);
				closesocket(clientContext->clientSocket);
				delete clientContext;
				continue;
			}
		}
	}
}

float Server::calculateFuelConsumption(TelemetryData previousTelemetryData, TelemetryData newTelemetryData)
{
	return previousTelemetryData.getFuel() - newTelemetryData.getFuel();
}
