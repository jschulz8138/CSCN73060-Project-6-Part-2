#include "Plane.h"

#define POUNDSPLANE "katl-kefd-B737-700.txt"

bool Plane::OpenFuelDataFile(std::string filename) {
    // set file directory with name and attempt open
    this->telemetryFile.open(dataFilePath + filename);
    // guard, check if open was successful
    if (!this->telemetryFile.is_open()) {
        std::cerr << "Couldn't open file: " << filename << std::endl;
        return false;
    }

    // checks if the plane telemetry is the one that stores
    // fuel as pouns.
    // Uses filename, not dynamic
    this->fuelType = filename == POUNDSPLANE ? FuelType::POUNDS : FuelType::GALLONS;

    // skips the first line (header)
    std::string line;
    std::getline(this->telemetryFile, line);

    return true;
}

bool Plane::GetNextFuelData() {
    // guard, check if open was successful
    if (!this->telemetryFile.is_open()) {
        std::cerr << "Telemetry data file is not open. cannot read yet." << std::endl;
        return false;
    }

    // set up line reader
    std::string line;
    // iterate through every line, sorting data into parameters
    while (std::getline(this->telemetryFile, line)) {
        // initiate string stream for parsing
        std::istringstream ss(line);
        std::string dateString, fuelString;
        // split line between date and fuel
        if (std::getline(ss, dateString, ',') && std::getline(ss, fuelString, ',')) {
            try {
                // store date and fuel quantity
                this->telemetry = TelemetryData(dateString.c_str(), stof(fuelString), this->fuelType);
                return true;
            }
            catch (std::exception e) {
                std::cerr << "Error parsing telemetry. " << e.what() << std::endl;
            }
        }
    }
    return false;
}

// close telemetry file
// DO NOT FORGET!!
void Plane::CloseFuelDataFile() {
    telemetryFile.is_open() ? telemetryFile.close() : void();
}

TelemetryData Plane::GetTelemetry()
{
    return this->telemetry;
}

FuelType Plane::GetFuelType()
{
    return this->fuelType;
}

std::string Plane::operator[](int index)
{
    // returns planefile name from the array if the index is within the array.
    return index < sizeof(this->planeFiles)/sizeof(this->planeFiles[0]) ? this->planeFiles[index] : "invalid file input";
}
