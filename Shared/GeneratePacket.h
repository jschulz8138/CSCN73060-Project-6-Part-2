// Definition of GeneratePacket Class

#pragma once
#include "Packet.h"

class GeneratePacket : public virtual Packet {
public:

	GeneratePacket(int);
	GeneratePacket(std::vector<char>);
	~GeneratePacket();

	std::vector<char> SerializeData() override;
	size_t size() const override;

	bool validateData() override;
	ProtocolFlag getFlag() const override;
	int getId() const override;
private:
	ProtocolFlag protocolFlag;
	int uniqueId;
};