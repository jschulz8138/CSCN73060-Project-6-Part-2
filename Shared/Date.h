#pragma once
#include <ctime>
#include <sstream>
#include <chrono>

class Date
{
public:
	//Constructor from a char*
	Date(const char* date = "1_1_1900 0:0:0");

	//Returns the date in string format the same as elliot's text files: 3_3_2023 14:53:22
	operator std::string() const;
	//Returns the length of the string that will be formed from this date
	size_t size() const;

private:
	time_t date;
};

