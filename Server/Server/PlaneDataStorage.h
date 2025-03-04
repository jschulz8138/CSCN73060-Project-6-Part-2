#pragma once
#include <atomic>
#include "../../Shared/TelemetryData.h"

class PlaneDataStorage
{
public:
	int generateNewId();
	bool storeAverage(int id);

private:
	std::atomic<int> currentID = 0;
	//Somesone of Storage. Maybe an undordered map? std::unordered_map<int, std::vector<TelementryData>> storedData;

};

