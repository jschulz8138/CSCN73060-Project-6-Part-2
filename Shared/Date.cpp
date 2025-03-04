#include "Date.h"

Date::Date(const char* date)
{
    struct tm tm = {};
    std::istringstream ss(date);
    std::chrono::sys_time<std::chrono::seconds> tp;
    ss >> std::chrono::parse("%d_%m_%Y %H:%M:%S", tp);          //3_3_2023 14:53:2
    if (ss.fail())
        throw std::invalid_argument("Invalid Date Format");

    this->date = std::chrono::system_clock::to_time_t(tp);
}

Date::operator std::string() const
{
    char buffer[80];
    struct tm* tm = localtime(&this->date);
    strftime(buffer, sizeof(buffer), "%d_%m_%Y %H:%M:%S", tm);
    return std::string(buffer);
}

size_t Date::size() const
{
    return std::string(*this).size();
}
