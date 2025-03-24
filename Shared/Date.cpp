#include "Date.h"

Date::Date(const char* date)     //Default date: "1_1_1900 0:0:0"
{
    struct tm tm = {};
    std::istringstream ss(date);
    std::chrono::sys_time<std::chrono::seconds> tp;
    ss >> std::chrono::parse("%m_%d_%Y %H:%M:%S", tp);          //3_3_2023 14:53:2
    if (ss.fail())
        throw std::invalid_argument("Invalid Date Format");

    this->date = std::chrono::system_clock::to_time_t(tp);
}

Date::operator std::string() const
{
    char buffer[80];
    struct tm* tm = localtime(&this->date);
    tm->tm_hour += tm->tm_isdst > 0 ? DST : EST;
    tm->tm_mday += tm->tm_hour / HOURS_IN_DAY;
    tm->tm_hour %= HOURS_IN_DAY;
    strftime(buffer, sizeof(buffer), "%m_%d_%Y %H:%M:%S", tm);
    return std::string(buffer);
}

size_t Date::size() const
{
    return std::string(*this).size();
}

bool Date::validateDate()
{
    struct tm defaultTm = {};
    defaultTm.tm_year = 1900 - 1900; //tm_year is years since 1900
    defaultTm.tm_mon = 0;  //January
    defaultTm.tm_mday = 1;
    defaultTm.tm_hour = 0;
    defaultTm.tm_min = 0;
    defaultTm.tm_sec = 0;

    time_t defaultTime = mktime(&defaultTm);
    return this->date != defaultTime;
}
