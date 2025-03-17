#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "..\Shared\Date.h"
#include "..\Shared\FuelType.h"
#include "..\Shared\TelemetryData.h"

class Plane
{
public:
	// Opens fuel file and set fuel type
	bool OpenFuelDataFile(std::string filename);
	// Gets the next entry of telemery
	// returns false if none is left
	bool GetNextFuelData();
	// Closes opened file!
	// DO NOT FORGET!!!!
	void CloseFuelDataFile();

	Date GetDate();
	float GetFuelQuantity();
	FuelType GetFuelType();

private:
	const std::string dataFilePath = "../Data Files/";
	TelemetryData telemetry;
	FuelType fuelType;
	std::ifstream telemetryFile;
};

