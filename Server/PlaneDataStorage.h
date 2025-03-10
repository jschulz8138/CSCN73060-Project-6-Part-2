#pragma once
#include <atomic>
#include <pqxx/pqxx>
#include <unordered_map>
#include <thread>
#include "../Shared/TelemetryData.h"

class PlaneDataStorage
{
public:
	int generateNewId();
	bool storeTelemetryData(pqxx::connection& conn, int id, TelemetryData telemetryData);
	bool storeFuelConsumption(pqxx::connection& conn, int id, float fuelConsumption, FuelType fuelType);
	bool storeAverageFuelConsumption(pqxx::connection& conn, int id);
	
	

private:
	std::atomic<int> currentID = 1;

	//PostGRE database methods
	bool insert(pqxx::connection& conn, const std::string& table, const std::vector<std::string>& columns, const std::vector<std::string>& values);	
	bool select(pqxx::connection& conn, const std::string& table, const std::string& condition, pqxx::result& result);
	bool setupDatabaseTables(pqxx::connection& conn);
};
