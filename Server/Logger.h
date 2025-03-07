#pragma once
#include <string>
#include <mutex>
#include <thread>
class Logger
{
public:
	bool logMessage(std::string message);
private:
	std::mutex logMutex;  //mutex for thread synchronization

};