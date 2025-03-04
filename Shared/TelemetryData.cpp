#include "TelemetryData.h"

TelemetryData::TelemetryData()
{
	this->date = Date();
	this->fuel = -1;
	this->fuelType = FuelType::OTHER;
}

TelemetryData::TelemetryData(std::vector<char> RxBuffer)
{
	char* BufferPtr = RxBuffer.data();

	//Deserialize date size
	size_t dateSize;
	std::memcpy(&dateSize, BufferPtr, sizeof(size_t));
	BufferPtr += sizeof(size_t);
	
	//Deserialize date string
	std::string dateStr;
	dateStr.resize(dateSize);
	std::memcpy(&dateStr[0], BufferPtr, dateSize);
	BufferPtr += dateSize;

	this->date = Date(dateStr.c_str());

	//Deserialize fuel
	std::memcpy(&this->fuel, BufferPtr, sizeof(float));
	BufferPtr += sizeof(float);

	//Deserialize fuel type
	std::memcpy(&this->fuelType, BufferPtr, sizeof(int));
}

TelemetryData::TelemetryData(Date date, float fuel, FuelType fuelType)
{
	this->date = date;
	this->fuel = fuel;
	this->fuelType = fuelType;
}

std::vector<char> TelemetryData::SerializeData()
{
	std::vector<char> TxBuffer(this->size());
	char* BufferPtr = TxBuffer.data();

	//Copy date size
	size_t dateSize = this->date.size();
	std::memcpy(BufferPtr, &dateSize, sizeof(size_t));
	BufferPtr += sizeof(size_t);

	//Copy date string
	std::string dateStr = this->date;
	std::memcpy(BufferPtr, dateStr.c_str(), dateSize);
	BufferPtr += dateSize;

	//Copy fuel
	std::memcpy(BufferPtr, &this->fuel, sizeof(float));
	BufferPtr += sizeof(float);

	//Copy fuel type
	std::memcpy(BufferPtr, &this->fuelType, sizeof(int));
	return TxBuffer;
}

size_t TelemetryData::size() const
{
	return sizeof(size_t) + this->date.size() + sizeof(float) + sizeof(int); 
}

bool TelemetryData::validateTeletryData()
{
	//Basically, just make sure that all attributes are not null, not zero, not a data of 01/01/1900 etc
	return true;
}