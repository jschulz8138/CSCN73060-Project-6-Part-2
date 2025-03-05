#include "Packet.h"


Packet::Packet(std::vector<char> RxBuffer)
{
	char* BufferPtr = RxBuffer.data();

	//Deserialize ProtocolFlag
	std::memcpy(&this->protocolFlag, BufferPtr, sizeof(int));
	BufferPtr += sizeof(int);

	// Deserialize UniqueID
	std::memcpy(&this->uniqueId, BufferPtr, sizeof(int));
	BufferPtr += sizeof(int);

	//Deserialize Telemetry Data
	std::vector<char> telemetryBuffer(BufferPtr, BufferPtr + (RxBuffer.size() - (BufferPtr - RxBuffer.data())));
	this->telementryData = TelemetryData(telemetryBuffer);
}

Packet::Packet(ProtocolFlag protocolFlag, int uniqueId, TelemetryData telementryData)
{
	this->protocolFlag = protocolFlag;
	this->uniqueId = uniqueId;
	this->telementryData = telementryData;
}

std::vector<char> Packet::SerializeData()
{
	std::vector<char> TxBuffer(this->size());
	char* BufferPtr = TxBuffer.data();
	std::memcpy(BufferPtr, &this->protocolFlag, sizeof(int));
	BufferPtr += sizeof(int);

	std::memcpy(BufferPtr, &this->uniqueId, sizeof(int));
	BufferPtr += sizeof(int);

	std::vector<char> telemetryBuffer = this->telementryData.SerializeData();
	std::memcpy(BufferPtr, telemetryBuffer.data(), telementryData.size());
	return TxBuffer;
}

size_t Packet::size() const
{
	return sizeof(int) + sizeof(int) + this->telementryData.size();	//Protocol Flag, UniqueId, Telemetry Data
}

bool Packet::validateTelemetryData()
{
	return true;
}

ProtocolFlag Packet::getFlag() const {
	return this->protocolFlag;
}

int Packet::getId() const {
	return this->uniqueId;
}

TelemetryData Packet::getTelemetryData()
{
	return this->telementryData;
}
