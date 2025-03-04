#include "Client.h"

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

bool Client::Connect() {
    sockaddr_in serverAddr = {};
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(SERVER_PORT);
    InetPtonA(AF_INET, "127.0.0.1", &serverAddr.sin_addr);

    if (connect(this->clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) != 0)
        return false;

    std::cout << "Connected to server." << std::endl;
    return true;
}

void Client::SendPacket(const Packet& packet) {
    std::vector<char> serializedData = packet.SerializeData();
    int bytesSent = send(this->clientSocket, serializedData.data(), serializedData.size(), 0);

    if (bytesSent == SOCKET_ERROR)
        throw std::runtime_error("Failed to send packet.");
}

Packet Client::ReceivePacket() {
    char buffer[BUFFER_SIZE];
    int bytesReceived = recv(this->clientSocket, buffer, BUFFER_SIZE, 0);

    if (bytesReceived <= 0)
        throw std::runtime_error("Failed to receive packet.");

    return Packet(std::vector<char>(buffer, buffer + bytesReceived));
}

void Client::Run() {
    if (!Connect()) {
        std::cerr << "Failed to connect to server." << std::endl;
        return;
    }

    // Receive Unique ID from Server
    Packet idPacket = ReceivePacket();
    int uniqueId = idPacket.getId();
    std::cout << "Received Unique ID: " << uniqueId << std::endl;

    // Send multiple data packets
    for (int i = 0; i < 5; i++) {
        TelemetryData telemetry(Date("3_4_2025"), i * 10.0f, FuelType::GALLONS);  // Example data
        Packet dataPacket(ProtocolFlag::SENDDATA, uniqueId, telemetry);
        SendPacket(dataPacket);

        // Wait for acknowledgment
        Packet ackPacket = ReceivePacket();
        if (ackPacket.getFlag() == ProtocolFlag::ACK)
            std::cout << "Received ACK for packet " << i + 1 << std::endl;
    }

    // End communication
    Packet endPacket(ProtocolFlag::ENDCOMMUNICATION, uniqueId, TelemetryData(Date("3_4_2025"), 10.0f, FuelType::GALLONS));  // Example data
    SendPacket(endPacket);

    std::cout << "Communication ended." << std::endl;
}