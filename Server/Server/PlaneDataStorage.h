#pragma once
#include <atomic>
#include "../../Shared/TelemetryData.h"

class PlaneDataStorage
{
public:
	int generateNewId();
	bool storeTelemetryData(int id, TelemetryData telemetryData);
	bool storeAverage(int id);

private:
	std::atomic<int> currentID = 1;
	//Somesone of Storage. Will need to use a database.

};

