#pragma once
#include <atomic>
#include "../Shared/TelemetryData.h"

class PlaneDataStorage
{
public:
	int generateNewId();
	bool storeTelemetryData(int id, TelemetryData telemetryData);
	bool storeAverage(int id);
	
	//PostGRE Statements
	bool insert();	//The insert statement should take a parameter of the database connection.
	bool select();	//The select statement should take a parameter of the database connection.

private:
	std::atomic<int> currentID = 1;
	
};
