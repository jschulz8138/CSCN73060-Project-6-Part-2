// Definition of SendPacket Class

#pragma once
#include "Packet.h"

class SendPacket : public virtual Packet {
public:

	SendPacket(int, TelemetryData);
	SendPacket(std::vector<char>);
	~SendPacket();

	std::vector<char> SerializeData() override;
	size_t size() const;

	bool validateData() override;
	ProtocolFlag getFlag() const override;
	int getId() const;
	TelemetryData getTelemetryData();

private:
	ProtocolFlag protocolFlag;
	int uniqueId;
	TelemetryData telemetryData;
};

