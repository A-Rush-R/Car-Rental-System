#ifndef USER_H
#define USER_H

#include <string>
#include <iostream>
#include<vector>
#include <algorithm>
#include "Car.h"
using namespace std;
#define CUSTOMER_BEGIN_ID 200001

class User {
protected:
    std::string password;

public:
    std::string name;
    int id;

    User(std::string name, int id, std::string password);
    virtual void show() = 0;
};

class Customer : public User 
{
        int fine_due;
    public : 
        vector<int> rented_cars;
        int record ;

        void set_password(string pass);
        Customer(const string& name, int id, const string& password, int fine_due, int record,const vector<int>& rented_cars);
		int show_due();
        void clear_due();
        void show();
        static void addCustomer(vector<Customer>& customers);
        static Customer* login(vector<Customer>& customers);
        static void updateCustomer(vector<Customer>& Customers);
        static void deleteCustomer(vector<Customer>& Customers);
        static void show_customers(vector<Customer>& customers);
        static Customer* searchCustomerById(vector<Customer>& Customers, int CustomerId);
        void rent_request(vector<Car>& cars);
        void begin_session(vector<Car>& cars);
        static void saveToFile(const vector<Customer>& customers, const string& filename);
        static void loadFromFile(vector<Customer>& customers, const string& filename);
};

#endif // USER_H
