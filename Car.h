#ifndef CAR_H
#define CAR_H

#include <iostream>
#include <vector>
#include <fstream>
#include <cassert>
#include <algorithm>
#include <regex>
#include "dateTime.h" 

class Car {
public:
    int rent;
    DateTime due_date, rent_date;
    int id;
    std::string model;
    int condition;
    int ownerID;

    Car(std::string model = "", int id = 0, int condition = 0, int rent = 0, DateTime due_date = DateTime(), DateTime rent_date = DateTime(), int ownerID = 0);

    void show(int discount) const;
    int rent_request(int id, int y, int m, int d, int y_, int m_, int d_);
    DateTime show_due_date();
    void repair();
    static void addCar(std::vector<Car>& cars);
    static void updateCar(std::vector<Car>& cars);
    static void deleteCar(std::vector<Car>& cars);
    static Car* searchCarById(std::vector<Car>& cars, int carId);
    static void showcars(std::vector<Car>& cars, int userID);
    static void saveToFile(const std::vector<Car>& cars, const std::string& filename);
    static void loadFromFile(std::vector<Car>& cars, const std::string& filename);
};

#endif // CAR_H
