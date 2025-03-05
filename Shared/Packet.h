#pragma once
#include "Date.h"
#include "TelemetryData.h"
#include "ProtocolFlag.h"
#include <string>

class Packet
{
public:

	Packet(ProtocolFlag protocolFlag, int uniqueId, TelemetryData telementryData);
	Packet(std::vector<char> RxBuffer);

	std::vector<char> SerializeData();
	size_t size() const;

	bool validateTelemetryData();
	ProtocolFlag getFlag() const;
	int getId() const;
	TelemetryData getTelemetryData();

private:
	ProtocolFlag protocolFlag;
	int uniqueId;
	TelemetryData telementryData;
};

