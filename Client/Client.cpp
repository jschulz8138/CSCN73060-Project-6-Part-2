#include "Client.h"
#include "Plane.h"

Client::Client() {
    WSAData wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
        throw std::runtime_error("Failed to start WSA.");

    this->clientSocket = socket(AF_INET, SOCK_STREAM, 0);
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
    int bytesSent = send(this->clientSocket, serializedData.data(), serializedData.size(), 0);

    if (bytesSent == SOCKET_ERROR)
        throw std::runtime_error("Failed to send packet.");
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
    fileOpened = plane.OpenFuelDataFile("Packet dataPacket;") ? true : false;

    // reads through file if lines are left
    while (fileOpened && plane.GetNextFuelData()) {
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