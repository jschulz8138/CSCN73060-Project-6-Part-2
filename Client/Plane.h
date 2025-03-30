#pragma once
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include "..\Shared\Date.h"
#include "..\Shared\FuelType.h"
#include "..\Shared\TelemetryData.h"

#define NUM_OF_PLANE_FILES 4

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

	// getter for telemtry variable
	TelemetryData GetTelemetry();
	// getter for fuel type variable
	FuelType GetFuelType();

	// operator overload, selects of the plane files
	std::string operator[](int index);

private:
	const char* WhiteSpace = " \t\v\r\n";
	const std::string dataFilePath = "Data Files/";
	TelemetryData telemetry;
	FuelType fuelType;
	std::ifstream telemetryFile;

	const std::string planeFiles[NUM_OF_PLANE_FILES] = {
		"katl-kefd-B737-700.txt",
		"Telem_2023_3_12 14_56_40.txt",
		"Telem_2023_3_12 16_26_4.txt",
		"Telem_czba-cykf-pa28-w2_2023_3_1 12_31_27.txt"
	};

};

