// Implementation of PacketFactory Class

#include "PacketFactory.h"

// Factory methods to create packets
Packet PacketFactory::create(ProtocolFlag flag, int id, TelemetryData Data) {
    
    // check the flag and create the according packet
    switch (flag) {
    case ProtocolFlag::GENERATEID: {
        GeneratePacket packet(id);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::SENDDATA: {
        SendPacket packet(id, Data);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ENDCOMMUNICATION: {
        EndPacket packet(id);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ACK: {
        AckPacket packet;
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    default: {//Default will just throw an error.
        throw std::invalid_argument("Invalid ProtocolFlag");
    }
    }
}

Packet PacketFactory::create(std::vector<char> RxBuffer) { // deserializing constructor function

    // initialize a flag variable to put the deserialized falg into
    ProtocolFlag flag;
    char* BufferPtr = RxBuffer.data();

    //Deserialize ProtocolFlag to determine further deserialization
    std::memcpy(&flag, BufferPtr, sizeof(int));

    // check the flag and create the according packet (using the deserializing constructor)
    switch (flag) {
    case ProtocolFlag::GENERATEID: {
        GeneratePacket packet(RxBuffer);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::SENDDATA: {
        SendPacket packet(RxBuffer);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ENDCOMMUNICATION: {
        EndPacket packet(RxBuffer);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    case ProtocolFlag::ACK: {
        AckPacket packet(RxBuffer);
        return packet.validateData() ? packet : throw std::invalid_argument("Packet could not validateData()");
    }

    default: {//Default will just throw an error.
        throw std::invalid_argument("Invalid ProtocolFlag");
    }
}