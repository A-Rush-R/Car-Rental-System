#ifndef DATETIME_H
#define DATETIME_H

class DateTime {
private:
    int year;
    int month;
    int day;

public:
    DateTime(int yr = 0, int mon = 0, int d = 0);
    DateTime(const DateTime& other);
    int getYear() const;
    int getMonth() const;
    int getDay() const;
    void display() const;
    static int daysInMonth(int year, int month);
    static bool isLeapYear(int year);
};

#endif // DATETIME_H
