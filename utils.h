#ifndef UTILS_H
#define UTILS_H

#include <regex>
#include <sstream>
#include <string>
#include "DateTime.h"

bool parse_date(std::string date, int* d, int* m, int* y);
int operator-(const DateTime& date1, const DateTime& date2);

#endif // UTILS_H
