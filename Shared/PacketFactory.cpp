// Implementation of PacketFactory Class

#include "PacketFactory.h"

// Factory methods to create packets
Packet PacketFactory::create(ProtocolFlag flag, int id, TelemetryData Data) {
    
    // check the flag and create the according packet
    switch (flag) {
    case ProtocolFlag::GENERATEID:
        return GeneratePacket(id);
        break;

    case ProtocolFlag::SENDDATA:
        return SendPacket(id, Data);
        break;

    case ProtocolFlag::ENDCOMMUNICATION:
        return EndPacket(id);
        break;

    case ProtocolFlag::ACK:
        return AckPacket();
        break;

    default: // default being ACK as that causes the least amount of issues (i think)
        return AckPacket();
        break;
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
    case ProtocolFlag::GENERATEID:
        return GeneratePacket(RxBuffer);
        break;

    case ProtocolFlag::SENDDATA:
        return SendPacket(RxBuffer);
        break;

    case ProtocolFlag::ENDCOMMUNICATION:
        return EndPacket(RxBuffer);
        break;

    case ProtocolFlag::ACK:
        return AckPacket(RxBuffer);
        break;

    default: // default being ACK as that causes the least amount of issues (i think)
        return AckPacket(RxBuffer);
        break;
    }
}