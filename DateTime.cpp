#include "DateTime.h"
#include <iostream>
#include <iomanip>

DateTime::DateTime(int yr, int mon, int d)
    : year(yr), month(mon), day(d) {}

DateTime::DateTime(const DateTime& other) {
    year = other.year;
    month = other.month;
    day = other.day;
}

int DateTime::getYear() const {
    return year;
}

int DateTime::getMonth() const {
    return month;
}

int DateTime::getDay() const {
    return day;
}

void DateTime::display() const {
    std::cout << std::setfill('0') << std::setw(4) << year << '-'
              << std::setw(2) << month << '-'
              << std::setw(2) << day << std::endl;
}
