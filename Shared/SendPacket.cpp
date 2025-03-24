// Implementation of SendPacket Class

#include "SendPacket.h"


SendPacket::SendPacket(int uniqueId, TelemetryData telemetryData)
{
	this->protocolFlag = ProtocolFlag::SENDDATA;
	this->uniqueId = uniqueId;
	this->telemetryData = telemetryData;
}

SendPacket::SendPacket(std::vector<char> RxBuffer)
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
	this->telemetryData = TelemetryData(telemetryBuffer);
}

std::vector<char> SendPacket::SerializeData()
{

	// Need to Serialize Flag, ID, and TelemetryData for SendData
	std::vector<char> TxBuffer(this->size());
	char* BufferPtr = TxBuffer.data();
	std::memcpy(BufferPtr, &this->protocolFlag, sizeof(int));
	BufferPtr += sizeof(int);

	std::memcpy(BufferPtr, &this->uniqueId, sizeof(int));
	BufferPtr += sizeof(int);

	std::vector<char> telemetryBuffer = this->telemetryData.SerializeData();
	std::memcpy(BufferPtr, telemetryBuffer.data(), telemetryData.size());
	return TxBuffer;
}

size_t SendPacket::size() const
{
	return sizeof(ProtocolFlag) + sizeof(int) + this->telemetryData.size();	//Protocol Flag, UniqueId, Telemetry Data
}

bool SendPacket::validateData()
{
	return this->protocolFlag == ProtocolFlag::SENDDATA && this->uniqueId > 0 && this->telemetryData.validateTeletryData();
}
	
// ********** GETTERS **********
ProtocolFlag SendPacket::getFlag() const {
	std::ofstream MyFile("filename.txt");
	MyFile << this->protocolFlag;
	MyFile.close();
	return this->protocolFlag;
}

int SendPacket::getId() const {
	return this->uniqueId;
}

TelemetryData SendPacket::getTelemetryData()
{
	return this->telemetryData;
}

SendPacket::~SendPacket() {
	//No extra cleanup needed. Default is fine.
}