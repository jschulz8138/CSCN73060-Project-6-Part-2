#include "PlaneDataStorage.h"
#include <iostream>

int PlaneDataStorage::generateNewId()
{
    //std::cout << "Assigning ID :" << this->currentID - 1 << std::endl;
	return this->currentID++;
}

bool PlaneDataStorage::storeTelemetryData(pqxx::connection& conn, int id, TelemetryData telemetryData)
{
	std::string dateString = telemetryData.getDate();
	return insert(
		conn,
		"telemetry",
		{"id", "date", "fuel", "fuel_type"},
		{std::to_string(id), dateString, std::to_string(telemetryData.getFuel()), std::to_string(static_cast<int>(telemetryData.getFuelType()))
		}
	);
}

bool PlaneDataStorage::storeFuelConsumption(pqxx::connection& conn, int id, float fuelConsumption, FuelType fuelType)
{
    return insert(
        conn,
        "fuel_consumption",
        { "id", "fuel_consumed", "fuel_type" },
        { std::to_string(id), std::to_string(fuelConsumption), std::to_string(static_cast<int>(fuelType)) }
    );
}


bool PlaneDataStorage::storeAverageFuelConsumption(pqxx::connection& conn, int id)
{
    pqxx::result result;

    //Select the fuel consumption values for the ID
    if (!select(conn, "fuel_consumption", "id = " + std::to_string(id), result)) {
        return false;  // No data found
    }

    float totalFuel = 0.0f;
    int count = 0;

    //Calculate the average fuel consumption
    for (auto row : result) {
        if (!row["fuel_consumed"].is_null()) {
            totalFuel += row["fuel_consumed"].as<float>();
            count++;
        }
    }

    // Insert the average into the database
    return count == 0 ? false : insert(
        conn,
        "average_fuel_consumption",
        { "id", "avg_fuel_consumed" },
        { std::to_string(id), std::to_string(totalFuel / count) }
    );
}


bool PlaneDataStorage::insert(pqxx::connection& conn, const std::string& table, const std::vector<std::string>& columns, const std::vector<std::string>& values)
{
    if (columns.size() != values.size()) {
        std::cout << "Bad Size" << std::endl;
        return false;
    }

    try {
        pqxx::work txn(conn);
        std::string sql = "INSERT INTO " + table + " (";

        for (size_t i = 0; i < columns.size(); ++i) {
            sql += columns[i] + (i < columns.size() - 1 ? ", " : "");
        }

        sql += ") VALUES (";
        for (size_t i = 0; i < values.size(); ++i) {
            sql += txn.esc(values[i]) + (i < values.size() - 1 ? ", " : "");
        }
        sql += ");";

        txn.exec(sql);
        txn.commit();
        return true;
    }
    catch (const std::exception& e) {
        std::cout << "err:" << e.what() << std::endl;
        return false;
    }

}

bool PlaneDataStorage::select(pqxx::connection& conn, const std::string& table, const std::string& condition, pqxx::result& result)
{
    try {
        pqxx::work txn(conn);
        std::string sql = "SELECT * FROM " + table + " WHERE " + condition + ";";

        result = txn.exec(sql);

        return !result.empty();  //Return true if data is found
    }
    catch (const std::exception& e) {
        return false;
    }
}

bool PlaneDataStorage::setupDatabaseTables(pqxx::connection& conn)
{
    try {
        pqxx::work txn(conn);

        // Create telemetry table
        txn.exec(
            "CREATE TABLE IF NOT EXISTS telemetry ("
            "id INT PRIMARY KEY, "
            "date TEXT NOT NULL, "
            "fuel FLOAT NOT NULL, "
            "fuel_type INT NOT NULL"
            ");"
        );

        // Create fuel consumption table
        txn.exec(
            "CREATE TABLE IF NOT EXISTS fuel_consumption ("
            "id INT NOT NULL, "
            "fuel_consumed FLOAT NOT NULL, "
            "fuel_type INT NOT NULL"
            ");"
        );

        // Create average fuel consumption table
        txn.exec(
            "CREATE TABLE IF NOT EXISTS average_fuel_consumption ("
            "id INT PRIMARY KEY, "
            "avg_fuel_consumed FLOAT NOT NULL"
            ");"
        );

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cout << "Setup failed:"<< e.what() << std::endl;
        return false;
    }
    std::cout << "Database Table Setup succeeded" << std::endl;
    return true;
}

bool PlaneDataStorage::setupData(pqxx::connection& conn)
{
    try {
        pqxx::work txn(conn);
        pqxx::result result = txn.exec("SELECT MAX(id) FROM average_fuel_consumption;");

        if (!result.empty() && !result[0][0].is_null()) {
            this->currentID = result[0][0].as<int>() + 1; // next available ID
        }
        else {
            this->currentID = 1; // start from 1 if table is empty
        }

        txn.commit();
    }
    catch (const std::exception& e) {
        std::cerr << "Failed to initialize currentID from database: " << e.what() << std::endl;
        this->currentID = 1; // fallback
        return false;
    }
    std::cout << "Database ID Setup succeeded." << std::endl;
    return true;
}
