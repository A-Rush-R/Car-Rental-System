#include<iostream>
#include<regex>
#include "utils.h"
using namespace std;

bool parse_date(string date, int* d, int* m, int* y) {
    regex pattern("\\b\\d{2}-\\d{2}-\\d{4}\\b");
    char delimiter;
    istringstream iss(date);
    if (regex_match(date, pattern)) {
        iss >> *d >> delimiter >> *m >> delimiter >> *y;
        // Check if month is valid (between 1 and 12)
        if (*m < 1 || *m > 12) {
            cout << "Invalid month. Month must be between 1 and 12." << endl;
            return false;
        }
        // Check if day is valid for the given month and year
        int max_days = DateTime::daysInMonth(*y, *m);
        if (*d < 1 || *d > max_days) {
            cout << "Invalid day. Day must be between 1 and " << max_days << " for the given month and year." << endl;
            return false;
        }
        return true;
    } else {
        cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
        return false;
    }
}

int operator-(const DateTime& date1, const DateTime& date2) {
    int days1 = date1.getYear() * 365 + (date1.getYear() / 4 - date1.getYear() / 100 + date1.getYear() / 400);
    for (int m = 1; m < date1.getMonth(); ++m) {
        days1 += DateTime :: daysInMonth(date1.getYear(), m);
    }
    days1 += date1.getDay();
    int days2 = date2.getYear() * 365 + (date2.getYear() / 4 - date2.getYear() / 100 + date2.getYear() / 400);
    for (int m = 1; m < date2.getMonth(); ++m) {
        days2 += DateTime :: daysInMonth(date2.getYear(), m);
    }
    days2 += date2.getDay();
    return days1 - days2;
}

void exit()
{
    cout << "Exiting..." << endl;
}

void non_int()
{
    cout << "Please enter an integer" << endl;
}

void invalid_option()
{
    cout << "Invalid Option" << endl;
}
