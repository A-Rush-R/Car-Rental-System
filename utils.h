#ifndef UTILS_H
#define UTILS_H

#include <regex>
#include <sstream>
#include <string>
#include "dateTime.h"

bool parse_date(std::string date, int* d, int* m, int* y);
int operator-(const DateTime& date1, const DateTime& date2);
void exit();
void non_int();
void invalid_option();
#endif // UTILS_H
