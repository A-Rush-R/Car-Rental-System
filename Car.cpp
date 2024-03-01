#include "Car.h"
#include<iostream>
#include<sstream>  
#include<string>
#include<fstream>
#include<vector>
#include<algorithm>
#include "DateTime.h"
#include "utils.h"
#include<cassert>
#include<iomanip>
#define CAR_BEGIN_ID 100001
#define MANAGER_BEGIN_ID 400001
#define HEAVY_DAMAGE 1
#define LIGHT_DAMAGE 2
#define MINOR_SCRATCHES 3
#define FINE 4 
using namespace std;

Car::Car(std::string model, int id, int condition, int rent, DateTime due_date, DateTime rent_date, int ownerID)
    : id(id), model(model), condition(condition), rent(rent), due_date(due_date), rent_date(rent_date), ownerID(ownerID) {}

void Car :: repair()
{
    condition = FINE;
}

void Car :: show() const {
    string status = ownerID == 0 ? "Available" : "Rented";
    // cout << "ID   Model    Condition    Status    OwnerID  Rent(per day)" << std::endl;
    string condition_name;

    assert(condition <= FINE && condition >= HEAVY_DAMAGE);

    switch (condition) {
        case HEAVY_DAMAGE:
            condition_name = "Heavy Damage";
            break;
        case LIGHT_DAMAGE:
            condition_name = "Light Damage";
            break;
        case MINOR_SCRATCHES:
            condition_name = "Minor Scratches";
            break;
        case FINE:
            condition_name = "Fine";
            break;
    }
    cout << left << setw(8) << id << setw(15) << model << setw(20) << condition_name << setw(12) << status << setw(7) << rent ;
    // cout << id << " " << model << " " << condition_name << " " << status << " "  << rent << " ";
    if(ownerID != 0)  
        cout << setw(8) << ownerID  << setw(11) << rent_date.display() << setw(10) <<  due_date.display();
    cout << endl;
}

int Car::rent_request(int id, int y, int m, int d, int y_, int m_, int d_) {
    ownerID = id;
    rent_date = DateTime(y, m, d);
    due_date = DateTime(y_, m_, d_);
    return 0;
}

DateTime Car::show_due_date() {
    return due_date;
}

// Function to add a new car to the database
void Car::addCar(std::vector<Car>& cars) {
    int id, condition;
    std::string model;
    std::cout << "Enter the model of the Car to be Added : ";
    std::cin >> model;

    if (!cars.empty())
        cars.push_back(Car(model, id = cars.back().id + 1));
    else
        cars.push_back(Car(model, id = CAR_BEGIN_ID));

    std::cout << "The ID of the added car is " << id << std::endl;
}

// Function to update an existing car in the database
void Car::updateCar(std::vector<Car>& cars) {
    int carId;
    cout << "Enter the ID of the car to be modified : " ;
    cin >> carId;

    int k;

    auto it = lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
        return car.id < id;
    });
    if (it != cars.end()) {
        
        cout << "Choose the entry to modify\n1 - Condition\n2 - Rent-Date\n3 - Due-Date\n4 - Rent" << endl;
        cin >> k;
        string temp;
        int d,m,y;
        regex pattern("\\b\\d{2}-\\d{2}-\\d{4}\\b");
        switch(k){
            case 1:
                cout << "Choose an option\n1 - Heavy Damaged\n2 - Light Damage\n3 - Minor Scratches\n4 - Fine" << endl;
                cin >> k;
                it->condition = k;                
                break;
            case 2:
                cout << "Enter the rent-date of the Car (in DD-MM-YYYY) format : " ;
                cin >> temp;
                if (!parse_date(temp,&d,&m,&y))
                {
                    cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                    updateCar(cars);
                }
                it->rent_date = DateTime(y,m,d);
                break;
            case 3:
                cout << "Enter the due-date of the Car (in DD-MM-YYYY) format : " ;
                cin >> temp;
            if (!parse_date(temp,&d,&m,&y))
            {
                cout << "Invalid date format. Please enter date in DD-MM-YYYY format." << endl;
                updateCar(cars);
            }
            it->due_date = DateTime(y,m,d);
                break;
            case 4 :
                cout << "Enter the new rent to set : ";
                cin >> k;
                it->rent = k;
                break;
            default:
                cout << "Please enter a valid number!" << endl;
                updateCar(cars);
                break;
        }
        // *it = updatedCar;

    } else {
        cout << "Car with ID " << carId << " not found." << endl;
    }
}

// Function to delete a car from the database
void Car::deleteCar(std::vector<Car>& cars) {
    int carId;
    cout << "Enter the ID of the car to be deleted : " ;
    cin >> carId;

    auto it = lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
        return car.id < id;
    });

    if (it != cars.end()) {
        cars.erase(it);
    } else {
        cout << "Car with ID " << carId << " not found." << endl;
    }
}

// Function to search for a car by ID
Car* Car::searchCarById(std::vector<Car>& cars, int carId) {
    auto it = lower_bound(cars.begin(), cars.end(), carId, [](const Car& car, int id) {
        return car.id < id;
    });

    if (it != cars.end() && it->id == carId) {
        return &(*it); 
    } else {
        return nullptr; 
    }
}

void Car :: showcars(vector<Car>& cars, int userID) {
    int count = 0;
    if(userID / MANAGER_BEGIN_ID == 1){//for manager
        if(!cars.size())
            cout << "No cars available" << endl;
        else {
            cout << left << setw(8) << "ID" << setw(15) << "Model" << setw(20) << "Condition" << setw(12) << "Status" << setw(7) << "Rent" << setw(8) << "OwnerID" << setw(11) << "Rent-Date" << setw(10) << "Due-Date" << endl;

            for(auto& it : cars){
                it.show();
            }
        }
    }
    else
    {   for(auto& it : cars)
        {
            if(it.ownerID == userID || it.ownerID == 0){
                if (count == 0)
                    cout << left << setw(8) << "ID" << setw(15) << "Model" << setw(20) << "Condition" << setw(12) << "Status" << setw(7) << "Rent" << setw(8) << "OwnerID" << setw(11) << "Rent-Date" << setw(10) << "Due-Date" << endl;

                it.show();
                count ++;
            }
        }
        if (count == 0)
            cout << "No cars available" << endl;
    }
}

void Car :: saveToFile(const vector<Car>& cars, const string& filename) {
    ofstream outFile(filename);
    if (outFile.is_open()) {
        for (const auto& car : cars) {
            outFile << car.id << " " << car.model << " " << car.condition << " " << car.ownerID << " " << car.rent << " " << car.rent_date.getYear() << " " << car.rent_date.getMonth() << " " << car.rent_date.getDay() << " " << car.due_date.getYear() << " " << car.due_date.getMonth() << " " << car.due_date.getDay() << endl;
            // cout << car.id << " " << car.model << " " << car.condition << endl;
        }
        outFile.close();
        cout << "Records saved to " << filename << endl;
    } else {
        cerr << "Unable to open file " << filename << endl;
    }
}

void Car :: loadFromFile(std::vector<Car>& cars, const std::string& filename){
    ifstream inFile(filename);
    if (inFile.is_open()) { 
        cars.clear(); // Clear existing data
        int id, condition, ownerID, rent;
        string model;
        int y,m,d;
        int y_,m_,d_;

        while (inFile >> id >> model >> condition >> ownerID >> rent >> y_ >> m_ >> d_ >> y >> m >> d) {
            cars.push_back(Car(model,id,condition,rent,DateTime(y_,m_,d_),DateTime(y,m,d),ownerID));
        }
        inFile.close();
        cout << "Records loaded from " << filename << endl;
    } else {
    cerr << "Unable to open file " << filename << endl;
    }
}
