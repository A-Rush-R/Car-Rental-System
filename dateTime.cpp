#include "dateTime.h"
#include <iostream>
#include <iomanip>
#include <sstream>
using namespace std;

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

string DateTime::display() const {
    std::ostringstream oss;
    oss << setfill('0') << setw(2) << day  << '-'<< setw(2) << month << '-' << setw(4) << year;
    return oss.str();
}

// Helper function to check if a year is a leap year
bool DateTime :: isLeapYear(int year) {
    return (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);
}

// Helper function to get the number of days in a month
int DateTime :: daysInMonth(int year, int month) {
    switch (month) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            return 31;
        case 4: case 6: case 9: case 11:
            return 30;
        case 2:
            return isLeapYear(year) ? 29 : 28;
        default:
            cerr << "Invalid Month" << endl;
            return 0; // Invalid month
    }
}