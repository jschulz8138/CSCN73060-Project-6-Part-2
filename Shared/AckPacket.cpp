// Implementation of AckPacket Class

#include "AckPacket.h"

AckPacket::AckPacket()
{
	this->protocolFlag = ProtocolFlag::ACK;
}

AckPacket::AckPacket(std::vector<char> RxBuffer)
{
	char* BufferPtr = RxBuffer.data();

	//Deserialize ProtocolFlag
	std::memcpy(&this->protocolFlag, BufferPtr, sizeof(int));
}

std::vector<char> AckPacket::SerializeData() {

	// Only need to Serialize Flag for ACK
	std::vector<char> TxBuffer(size());
	char* BufferPtr = TxBuffer.data();

	std::memcpy(BufferPtr, &protocolFlag, sizeof(int));

	return TxBuffer;
}

size_t AckPacket::size() const{
	return sizeof(ProtocolFlag); 	//Protocol Flag
}

// ********** GETTERS **********
bool AckPacket::validateData() {
	return this->protocolFlag == ProtocolFlag::ACK;
}

ProtocolFlag AckPacket::getFlag() const{
	return this->protocolFlag;
}

AckPacket::~AckPacket() {
	//No extra cleanup needed. Default is fine.
}