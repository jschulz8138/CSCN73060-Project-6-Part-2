// Definition of AckPacket Class

#pragma once
#include "Packet.h"

class AckPacket : public Packet {
public:

	AckPacket();
	AckPacket(std::vector<char>);
	~AckPacket();

	std::vector<char> SerializeData() override;
	size_t size() const override;

	bool validateData() override;
	ProtocolFlag getFlag() const override;

private:
	ProtocolFlag protocolFlag;
};