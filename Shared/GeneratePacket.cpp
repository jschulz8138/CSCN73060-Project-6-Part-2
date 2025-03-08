// Implementation of GeneratePacket Class

#include "GeneratePacket.h"

GeneratePacket::GeneratePacket(int id)
{
	this->protocolFlag = ProtocolFlag::GENERATEID;
	this->uniqueId = id;
}

GeneratePacket::GeneratePacket(std::vector<char> RxBuffer)
{
	char* BufferPtr = RxBuffer.data();

	//Deserialize ProtocolFlag
	std::memcpy(&this->protocolFlag, BufferPtr, sizeof(int));
	BufferPtr += sizeof(int);

	// Deserialize UniqueID
	std::memcpy(&this->uniqueId, BufferPtr, sizeof(int));
}

std::vector<char> GeneratePacket::SerializeData()
{

	// Only need to Serialize Flag and ID for GenerateId
	std::vector<char> TxBuffer(this->size());
	char* BufferPtr = TxBuffer.data();

	std::memcpy(BufferPtr, &this->protocolFlag, sizeof(int));
	BufferPtr += sizeof(int);

	std::memcpy(BufferPtr, &this->uniqueId, sizeof(int));
	return TxBuffer;
}

size_t GeneratePacket::size() const
{
	return sizeof(int) + sizeof(int);	//Protocol Flag, UniqueId
}

bool GeneratePacket::validateTelemetryData()
{
	return true;
}

// ********** GETTERS **********
ProtocolFlag GeneratePacket::getFlag() const {
	return this->protocolFlag;
}

int GeneratePacket::getId() const {
	return this->uniqueId;
}

GeneratePacket::~GeneratePacket() {
	// Deconstructor stuff?
}