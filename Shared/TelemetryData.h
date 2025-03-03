#pragma once
#include "Date.h"
#include "FuelType.h"

class TelemetryData
{
public:
	TelemetryData();
	TelemetryData(std::vector<char> RxBuffer);
	TelemetryData(Date date, float fuel, FuelType fuelType);
	std::vector<char> SerializeData();
	size_t size() const;
private:
	Date date;
	float fuel;
	FuelType fuelType;
};

