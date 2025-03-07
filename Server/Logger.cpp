#include "Logger.h"
#include <iostream>
#include <fstream>
#include <string>
#include <ctime>
#include <mutex>
#include <thread>

const std::string LogFileName = "LogFile.csv";
const int TimeStringSize = 40;

bool Logger::logMessage(std::string message)
{
    //mutex for thread safety, lock guard automatically locks and releases the given lock
    std::lock_guard<std::mutex> guard(logMutex);

    std::ofstream logFile(LogFileName, std::ios::app);

    if (!logFile) {
        std::cerr << "Error opening the log file!" << std::endl;
        return false;
    }

    //get current time
    std::time_t currentTime = std::time(nullptr);
    std::tm timeInfo;
    if (localtime_s(&timeInfo, &currentTime) != 0) {
        std::cerr << "Error getting local time!" << std::endl;
        return false;
    }

    char timeStr[TimeStringSize];
    std::strftime(timeStr, sizeof(timeStr), "%Y-%m-%d %H:%M:%S", &timeInfo);

    //log message to the file (timestamp, message)
    //we can add more parameters easily 
    logFile << "\"" << timeStr << "\", \"" << message << "\"\n";

    logFile.close();

    return true;
}


