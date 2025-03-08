// Definition of SendPacket Class

#pragma once
#include "Packet.h"

class SendPacket : public Packet {
public:

	SendPacket(int, TelemetryData);
	SendPacket(std::vector<char>);
	~SendPacket();

	std::vector<char> SerializeData() override;
	size_t size() const;

	bool validateTelemetryData() override;
	ProtocolFlag getFlag() const override;
	int getId() const override;
	TelemetryData getTelemetryData() override;

private:
	ProtocolFlag protocolFlag;
	int uniqueId;
	TelemetryData telemetryData;
};

