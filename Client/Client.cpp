#include "Client.h"
#include "Plane.h"

Client::Client() {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::runtime_error("Failed to start WSA.");

    this->clientSocket = WSASocket(AF_INET, SOCK_STREAM, 0, nullptr, 0, WSA_FLAG_OVERLAPPED);
    if (this->clientSocket == INVALID_SOCKET)
        throw std::runtime_error("Failed to create socket.");
}

Client::~Client() {
    closesocket(this->clientSocket);
    WSACleanup();
}

bool Client::Connect(const char* serverIp) {
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    InetPtonA(AF_INET, serverIp, &serverAddr.sin_addr);

    if (connect(this->clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
        return false;

    std::cout << "Connected to server." << std::endl;
    return true;
}

void Client::SendPacket(std::unique_ptr<Packet>& packet) {
    std::vector<char> serializedData = packet->SerializeData();

    WSABUF wsaBuf;
    wsaBuf.buf = serializedData.data();
    wsaBuf.len = static_cast<ULONG>(serializedData.size());

    OVERLAPPED overlapped = {};
    HANDLE hEvent = CreateEvent(nullptr, TRUE, FALSE, nullptr);
    if (!hEvent)
        throw std::runtime_error("Failed to create event for WSASend.");

    overlapped.hEvent = hEvent;

    DWORD bytesSent = 0;
    int result = WSASend(this->clientSocket, &wsaBuf, 1, &bytesSent, 0, &overlapped, nullptr);

    if (result == SOCKET_ERROR) {
        int err = WSAGetLastError();
        if (err != WSA_IO_PENDING) {
            CloseHandle(hEvent);
            throw std::runtime_error("WSASend failed immediately." + std::to_string(err));
        }

        // Wait for the async operation to complete
        DWORD waitResult = WaitForSingleObject(hEvent, INFINITE);
        if (waitResult != WAIT_OBJECT_0) {
            CloseHandle(hEvent);
            throw std::runtime_error("WSASend wait failed.");
        }

        // Get the number of bytes actually sent
        if (!WSAGetOverlappedResult(this->clientSocket, &overlapped, &bytesSent, FALSE, nullptr)) {
            CloseHandle(hEvent);
            throw std::runtime_error("WSASend completion failed.");
        }
    }

    CloseHandle(hEvent);
}


std::unique_ptr<Packet> Client::ReceivePacket() {
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(this->clientSocket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived <= 0)
        throw std::runtime_error("Failed to receive packet.");

    std::unique_ptr<Packet> receivedPacket;
    try {
        receivedPacket = PacketFactory::create(std::vector<char>(buffer, buffer + bytesReceived));
    }
    catch (const std::exception& e) {
        std::cerr << "Failure while deserializing the recievedPacket";
    }

    return receivedPacket;
}

void Client::Run(const char* serverIp) {
    if (!Connect(serverIp)) {
        std::cerr << "Failed to connect to server." << std::endl;
        return;
    }

    // Receive Unique ID from Server
    std::unique_ptr<Packet> idPacket = ReceivePacket();
    int uniqueId = idPacket->getId();
    std::cout << "Received Unique ID: " << uniqueId << std::endl;
  
    // set up telemetry reading class
    Plane plane;
    bool fileOpened = true;



    // tries to open files, doesn't read if unsuccessful
    fileOpened = plane.OpenFuelDataFile(plane[0]) ? true : false;

    // reads through file if lines are left
    while (fileOpened && plane.GetNextFuelData()) {
        std::string str = plane.GetTelemetry().getDate();
        std::cout << str << std::endl;

        std::unique_ptr<Packet> dataPacket;
        try {
            dataPacket = PacketFactory::create(ProtocolFlag::SENDDATA, uniqueId, plane.GetTelemetry());
        }
        catch (const std::exception& e) {
            std::cerr << "Failure while deserializing the recievedPacket";
        }
      
        SendPacket(dataPacket);

        // Wait for acknowledgment
        std::unique_ptr<Packet> ackPacket = ReceivePacket();
        if (ackPacket->getFlag() == ProtocolFlag::ACK)
            std::cout << "Received ACK for packet" << std::endl;
    }

    // End communication
    std::unique_ptr<Packet> endPacket;
    try {
        endPacket = PacketFactory::create(ProtocolFlag::ENDCOMMUNICATION, uniqueId);  // Example data
    }
    catch (const std::exception& e) {
        std::cerr << "Failure while deserializing the recievedPacket";
    }

    SendPacket(endPacket);

    //There might need to be client cleanup here? I don't remember.

    std::cout << "Communication ended." << std::endl;
}