// Definition of EndPacket Class

#pragma once
#include "Packet.h"

class EndPacket : public Packet {
public:

	EndPacket(int);
	EndPacket(std::vector<char>);
	~EndPacket();

	std::vector<char> SerializeData() override;
	size_t size() const override;

	bool validateData() override;
	ProtocolFlag getFlag() const override;
	int getId() const override;

private:
	ProtocolFlag protocolFlag;
	int uniqueId;
};