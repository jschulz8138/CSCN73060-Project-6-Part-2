#include "PlaneDataStorage.h"

int PlaneDataStorage::generateNewId()
{
	return this->currentID++;
}

bool PlaneDataStorage::storeTelemetryData(int id, TelemetryData telemetryData)
{
	return true;
}

bool PlaneDataStorage::storeAverage(int id)
{
	return true;
}
