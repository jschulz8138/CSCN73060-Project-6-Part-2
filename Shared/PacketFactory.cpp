// Implementation of PacketFactory Class

#include "PacketFactory.h"
#include <iostream>

// Factory methods to create packets
std::unique_ptr<Packet> PacketFactory::create(ProtocolFlag flag, int id, TelemetryData Data) {
    
    // check the flag and create the according packet
    switch (flag) {
    case ProtocolFlag::GENERATEID: {
        std::unique_ptr<GeneratePacket> packet = std::make_unique< GeneratePacket>(id);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::SENDDATA: {
        std::unique_ptr<SendPacket> packet = std::make_unique<SendPacket>(id, Data);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ENDCOMMUNICATION: {
        std::unique_ptr<EndPacket> packet = std::make_unique<EndPacket>(id);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ACK: {
        std::unique_ptr<AckPacket> packet = std::make_unique<AckPacket>();
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    default: {//Default will just throw an error.
        throw std::invalid_argument("Invalid ProtocolFlag");
    }
    }
}

std::unique_ptr<Packet> PacketFactory::create(std::vector<char> RxBuffer) { // deserializing constructor function

    // initialize a flag variable to put the deserialized falg into
    ProtocolFlag flag;
    char* BufferPtr = RxBuffer.data();

    //Deserialize ProtocolFlag to determine further deserialization
    std::memcpy(&flag, BufferPtr, sizeof(int));

    // check the flag and create the according packet (using the deserializing constructor)
    switch (flag) {
    case ProtocolFlag::GENERATEID: {
        std::unique_ptr<GeneratePacket> packet = std::make_unique<GeneratePacket>(RxBuffer);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::SENDDATA: {
        std::unique_ptr<SendPacket> packet = std::make_unique<SendPacket>(RxBuffer);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ENDCOMMUNICATION: {
        std::unique_ptr<EndPacket> packet = std::make_unique<EndPacket>(RxBuffer);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ACK: {
        std::unique_ptr<AckPacket> packet = std::make_unique<AckPacket>(RxBuffer);
        return packet->validateData() ? std::move(packet) : throw std::invalid_argument("Packet could not validateData()");
    }

    default: {//Default will just throw an error.
        throw std::invalid_argument("Invalid ProtocolFlag");
    }
    }
}