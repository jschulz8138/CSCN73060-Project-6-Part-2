// Definition of Packet Abstract Class

#pragma once
#include "Date.h"
#include "TelemetryData.h"
#include "ProtocolFlag.h"
#include <string>

// ABSTRACT PACKET CLASS
class Packet
{
public:
	//Default destructor
	virtual ~Packet() = default;

	// Functions that (most) packets will use
	virtual std::vector<char> SerializeData() { return std::vector<char>(); };
	virtual size_t size() const { return 0; };

	virtual bool validateData() { return false; };

	virtual ProtocolFlag getFlag() const { return ProtocolFlag::ACK; };
	virtual int getId() const { return 0; };
	virtual TelemetryData getTelemetryData() { return TelemetryData(); };

};

