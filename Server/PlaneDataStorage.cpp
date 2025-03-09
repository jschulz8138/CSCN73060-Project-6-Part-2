#include "PlaneDataStorage.h"

int PlaneDataStorage::generateNewId()
{
	return this->currentID++;
}

bool PlaneDataStorage::storeTelemetryData(int id, TelemetryData telemetryData)
{
	return false;
}

bool PlaneDataStorage::storeAverage(int id)
{
	return false;
}

bool PlaneDataStorage::insert()
{
	return false;
}

bool PlaneDataStorage::select()
{
	return false;
}
