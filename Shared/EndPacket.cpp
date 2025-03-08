// Implementation of EndPacket Class

#include "EndPacket.h"

EndPacket::EndPacket(int id)
{
	this->protocolFlag = ProtocolFlag::ENDCOMMUNICATION;
	this->uniqueId = id;
}

EndPacket::EndPacket(std::vector<char> RxBuffer)
{
	char* BufferPtr = RxBuffer.data();

	//Deserialize ProtocolFlag
	std::memcpy(&this->protocolFlag, BufferPtr, sizeof(int));
	BufferPtr += sizeof(int);

	// Deserialize UniqueID
	std::memcpy(&this->uniqueId, BufferPtr, sizeof(int));
}

std::vector<char> EndPacket::SerializeData()
{
	// Only need to Serialize Flag and ID for EndCommunication
	std::vector<char> TxBuffer(this->size());
	char* BufferPtr = TxBuffer.data();

	std::memcpy(BufferPtr, &this->protocolFlag, sizeof(int));
	BufferPtr += sizeof(int);

	std::memcpy(BufferPtr, &this->uniqueId, sizeof(int));
	return TxBuffer;
}

size_t EndPacket::size() const
{
	return sizeof(ProtocolFlag) + sizeof(int);	//Protocol Flag, UniqueId
}

bool EndPacket::validateData()
{
	return this->protocolFlag == ProtocolFlag::ENDCOMMUNICATION && this->uniqueId > 0;
}

// ********** GETTERS **********
ProtocolFlag EndPacket::getFlag() const {
	return this->protocolFlag;
}

int EndPacket::getId() const {
	return this->uniqueId;
}

EndPacket::~EndPacket() {
	//No extra cleanup needed. Default is fine.
}