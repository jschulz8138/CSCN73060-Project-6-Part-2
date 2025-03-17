// Definition of PacketFactory Class

#pragma once
#include <vector>
#include "TelemetryData.h"
#include "GeneratePacket.h"
#include "SendPacket.h"
#include "EndPacket.h"
#include "AckPacket.h"

#include <memory>
#include <iostream>
#include <fstream>

// Defining a class to use the Factory Method to create packets
class PacketFactory {
public:
    // Deconstructor
    virtual ~PacketFactory() = default;

    // Factory methods to create packets
    static std::unique_ptr<Packet> create(ProtocolFlag, int = 0, TelemetryData = TelemetryData()); // Using default values to allow for some parameters to not be input
    static std::unique_ptr<Packet> create(std::vector<char>); // deserializing constructor function
};